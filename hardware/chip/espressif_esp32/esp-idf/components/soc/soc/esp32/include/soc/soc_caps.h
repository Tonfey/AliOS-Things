// The long term plan is to have a single soc_caps.h for each peripheral.
// During the refactoring and multichip support development process, we
// seperate these information into periph_caps.h for each peripheral and
// include them here.

#pragma once

#define SOC_MCPWM_SUPPORTED 1
#define SOC_SDMMC_HOST_SUPPORTED 1
#define SOC_BT_SUPPORTED 1
#define SOC_SDIO_SLAVE_SUPPORTED    1
#define SOC_TWAI_SUPPORTED 1
#define SOC_CAN_SUPPORTED SOC_TWAI_SUPPORTED
#define SOC_EMAC_SUPPORTED 1

#define SOC_CPU_CORES_NUM 2

// ESP32 have 2 I2S
#define SOC_I2S_NUM            (2)
