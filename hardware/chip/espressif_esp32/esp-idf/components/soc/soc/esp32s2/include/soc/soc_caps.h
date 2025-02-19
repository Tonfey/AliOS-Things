// The long term plan is to have a single soc_caps.h for each peripheral.
// During the refactoring and multichip support development process, we
// seperate these information into periph_caps.h for each peripheral and
// include them here.

#pragma once

#define SOC_TWAI_SUPPORTED 1
#define SOC_CPU_CORES_NUM 1
#define SOC_SUPPORTS_SECURE_DL_MODE 1

// ESP32-S2 have 2 I2S
#define SOC_I2S_NUM            (1)