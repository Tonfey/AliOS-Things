#include "mphalport.h"

#include <inttypes.h>
#include <unistd.h>

#include "aos/hal/uart.h"
#include "k_default_config.h"
#include "mpsalport.h"
#include "py/mpconfig.h"
#include "py/mphal.h"
#include "py/mpstate.h"
#include "py/obj.h"
#include "py/objstr.h"
#include "py/ringbuf.h"
#include "py/runtime.h"
#include "py/stream.h"

STATIC uint8_t stdin_ringbuf_array[260];
ringbuf_t stdin_ringbuf = { stdin_ringbuf_array, sizeof(stdin_ringbuf_array), 0, 0 };

static uart_dev_t uart_stdio = { 0 };

/*
 * Core UART functions to implement for a port
 */

uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags)
{
    uintptr_t ret = 0;
    if ((poll_flags & MP_STREAM_POLL_RD) && stdin_ringbuf.iget != stdin_ringbuf.iput) {
        ret |= MP_STREAM_POLL_RD;
    }
    return ret;
}

// Receive single character, Wait until we get UART input
int mp_hal_stdin_rx_chr(void)
{
#ifndef AOS_BOARD_HAAS700
    for (;;) {
        int c = ringbuf_get(&stdin_ringbuf);
        if (c != -1) {
            return c;
        }
        MICROPY_EVENT_POLL_HOOK
        mp_stdin_sem_take(1);  // Do not wait forever as print from mpthread want to work
    }
#else
    uint8_t c = 0;
    mp_uint_t recv_size = 0;
    int ret = -1;

    memset(&uart_stdio, 0, sizeof(uart_stdio));
    uart_stdio.port = 0;

    // try to check whether we have receive uart input
    ret = hal_uart_recv_II(&uart_stdio, &c, 1, &recv_size, 200);
    if (ret == 0 && recv_size == 1)
        return c;

    return -MP_EAGAIN;
#endif
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, size_t len)
{
#if MICROPY_VFS_POSIX || MICROPY_VFS_POSIX_FILE
    /* COPY LOGIC FROM UNIX IMPLEMENTATION
        vfs_posix_file_write will check MICROPY_PY_OS_DUPTERM MACRO and
        call this API directly
    */
    ssize_t ret;
    MP_HAL_RETRY_SYSCALL(ret, write(STDOUT_FILENO, str, len), {});
#else
    // Only release the GIL if many characters are being sent
    bool release_gil = len > 20;
    if (release_gil) {
        MP_THREAD_GIL_EXIT();
    }

    memset(&uart_stdio, 0, sizeof(uart_stdio));
    uart_stdio.port = 0;
    aos_hal_uart_send(&uart_stdio, str, len, 5000);

    if (release_gil) {
        MP_THREAD_GIL_ENTER();
    }
#endif

#if MICROPY_PY_OS_DUPTERM
    mp_uos_dupterm_tx_strn(str, len);
#endif
}

void mp_hal_stdout_tx_str(const char *str)
{
    mp_hal_stdout_tx_strn(str, strlen(str));
}

// Efficiently convert "\n" to "\r\n"
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len)
{
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}

mp_uint_t mp_hal_ticks_us(void)
{
    return krhino_ticks_to_ms(krhino_sys_tick_get()) * 1000;
}

mp_uint_t mp_hal_ticks_ms(void)
{
    return krhino_ticks_to_ms(krhino_sys_tick_get());
}

uint64_t mp_hal_time_ns(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t ns = tv.tv_sec * 1000000000ULL;
    ns += (uint64_t)tv.tv_usec * 1000ULL;
    return ns;
}

mp_uint_t mp_hal_ticks_cpu(void)
{
    return krhino_sys_tick_get();
}

void mp_hal_delay_us(mp_uint_t us)
{
    const uint32_t pend_overhead = 150;

    uint64_t t0 = aos_now_ms() * 1000;
    for (;;) {
        uint64_t dt = aos_now_ms() * 1000 - t0;
        if (dt >= us) {
            return;
        }
        if (dt + pend_overhead < us) {
            // we have enough time to service pending events
            // (don't use MICROPY_EVENT_POLL_HOOK because it also yields)
            mp_handle_pending(true);
        }
    }
}

void mp_hal_delay_ms(mp_uint_t ms)
{
    uint64_t us = ms * 1000;
    uint64_t dt;
    uint64_t t0 = aos_now_ms() * 1000;
    for (;;) {
        mp_handle_pending(true);
        MICROPY_PY_USOCKET_EVENTS_HANDLER
        MP_THREAD_GIL_EXIT();
        uint64_t t1 = aos_now_ms() * 1000;
        dt = t1 - t0;
        if (dt + RHINO_CONFIG_TICKS_PER_SECOND * 1000 >= us) {
            // doing a TaskDelay would take us beyond requested delay time
            aos_msleep(1);
            MP_THREAD_GIL_ENTER();
            t1 = aos_now_ms() * 1000;
            dt = t1 - t0;
            break;
        } else {
            mp_stdin_sem_take(1);
            MP_THREAD_GIL_ENTER();
        }
    }
    if (dt < us) {
        // do the remaining delay accurately
        mp_hal_delay_us(us - dt);
    }
}

// Wake up the main task if it is sleeping
void mp_hal_wake_main_task_from_isr(void)
{
    mp_stdin_sem_give();
}

/*******************************************************************************/
// GPIO
/*******************************************************************************/

gpio_dev_t mphal_gpio_config_obj_t[PY_GPIO_NUM_MAX] = { 0 };

mp_int_t mp_hal_pin_config_set(mp_hal_pin_obj_t pin_obj, gpio_config_t cfg)
{
    gpio_dev_t dev = { 0 };
    dev.port = pin_obj;
    dev.config = cfg;

    mp_int_t ret = aos_hal_gpio_init(&dev);
    if (0 != ret) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "pin index out range"));
        return ret;
    }

    mphal_gpio_config_obj_t[pin_obj] = dev;
    return ret;
}

gpio_dev_t mp_hal_gpio_dev_get(mp_hal_pin_obj_t pin_obj)
{
    if (pin_obj < 0 || pin_obj > PY_GPIO_NUM_MAX) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "pin index out range"));
        return;
    }
    return mphal_gpio_config_obj_t[pin_obj];
}

mp_int_t mp_hal_pin_high(mp_hal_pin_obj_t pin_obj)
{
    gpio_dev_t dev = { 0 };
    dev = mp_hal_gpio_dev_get(pin_obj);

    mp_int_t ret = aos_hal_gpio_output_high(&dev);
    if (0 != ret) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "mp_hal_pin_high fail"));
    }
    return ret;
}

mp_int_t mp_hal_pin_low(mp_hal_pin_obj_t pin_obj)
{
    gpio_dev_t dev = { 0 };
    dev = mp_hal_gpio_dev_get(pin_obj);

    mp_int_t ret = aos_hal_gpio_output_low(&dev);
    if (0 != ret) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "mp_hal_pin_low fail"));
    }
    return ret;
}

mp_int_t mp_hal_pin_read(mp_hal_pin_obj_t pin_obj)
{
    gpio_dev_t dev = { 0 };
    dev = mp_hal_gpio_dev_get(pin_obj);

    mp_int_t value = -1;
    mp_int_t ret = aos_hal_gpio_input_get(&dev, &value);
    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "mp_hal_pin_read fail"));
        return ret;
    }
    return value;
}

/*******************************************************************************/
// MAC address
/*******************************************************************************/
#define MP_HAL_UNIQUE_ID_ADDRESS (0xDEADBEAF)

// Generate a random locally administered MAC address (LAA)
void mp_hal_generate_laa_mac(int idx, uint8_t buf[6])
{
    uint8_t *id = (uint8_t *)MP_HAL_UNIQUE_ID_ADDRESS;
    buf[0] = 0x02;  // LAA range
    buf[1] = (id[11] << 4) | (id[10] & 0xf);
    buf[2] = (id[9] << 4) | (id[8] & 0xf);
    buf[3] = (id[7] << 4) | (id[6] & 0xf);
    buf[4] = id[2];
    buf[5] = (id[0] << 2) | idx;
}

// A board can override this if needed
MP_WEAK void mp_hal_get_mac(int idx, uint8_t buf[6])
{
    mp_hal_generate_laa_mac(idx, buf);
}

void mp_hal_get_mac_ascii(int idx, size_t chr_off, size_t chr_len, char *dest)
{
    static const char hexchr[16] = "0123456789ABCDEF";
    uint8_t mac[6];
    mp_hal_get_mac(idx, mac);
    for (; chr_len; ++chr_off, --chr_len) {
        *dest++ = hexchr[mac[chr_off >> 1] >> (4 * (1 - (chr_off & 1))) & 0xf];
    }
}
