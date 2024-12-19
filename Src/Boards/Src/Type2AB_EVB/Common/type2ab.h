/**
 * @file      type2ab.h
 *
 * @brief     Pin mapping description corresponding to Murata Type 2AB Module
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */


/**
 * +---------------------------------------------+ *
 * |     Pins connected to LIS2DW12              | *
 * +=============================================+ *
 * |  port&pin      |            Type            | *
 * +---------------------------------------------+ *
 * |  P0.19         |           SCL/SPC          | *
 * +---------------------------------------------+ *
 * |  P0.22         |           SDA/SDI/SDO      | *
 * +---------------------------------------------+ *
 * |  P1.1          |           INT1             | *
 * +---------------------------------------------+ *
 * |  P1.4          |           INT2             | *
 * +---------------------------------------------+ *
 * |  P1.3          |           CS               | *
 * +---------------------------------------------+ *
 * |  P1.0          |           SDO/SA0          | *
 * +---------------------------------------------+ *
 **/
#define LIS2DW12_IIC_SCL  NRF_GPIO_PIN_MAP(0, 19)
#define LIS2DW12_IIC_SDA  NRF_GPIO_PIN_MAP(0, 22)
#define LIS2DW12_INT1     NRF_GPIO_PIN_MAP(1, 1)
#define LIS2DW12_INT2     NRF_GPIO_PIN_MAP(1, 4)
#define LIS2DW12_SPI_CS   NRF_GPIO_PIN_MAP(1, 3)
#define LIS2DW12_SPI_MISO NRF_GPIO_PIN_MAP(1, 0)
#define LIS2DW12_SPI_SCLK NRF_GPIO_PIN_MAP(0, 19)
#define LIS2DW12_SPI_MOSI NRF_GPIO_PIN_MAP(0, 22)

/**
 * +---------------------------------------------+ *
 * |     Pins connected to crystal oscillator    | *
 * +=============================================+ *
 * |  port&pin      |            Type            | *
 * +---------------------------------------------+ *
 * |  XC1           |           32 MHz           | *
 * +---------------------------------------------+ *
 * |  XC2           |           32 MHz           | *
 * +---------------------------------------------+ *
 **/
#define EXTERNAL_32MHZ_CRYSTAL 1

#ifdef HAL_DW3000
/**
 * +---------------------------------------------+ *
 * |     Pins connected to DW3000                | *
 * +=============================================+ *
 * |  port&pin      |            Type            | *
 * +---------------------------------------------+ *
 * |  P0.20         |           SPI_CS           | *
 * +---------------------------------------------+ *
 * |  P0.16         |           SPI_CLK          | *
 * +---------------------------------------------+ *
 * |  P0.17         |           SPI_MOSI         | *
 * +---------------------------------------------+ *
 * |  P0.23         |           SPI_MISO         | *
 * +---------------------------------------------+ *
 * |  P0.15         |           RESET            | *
 * +---------------------------------------------+ *
 * |  P0.25         |           IRQ              | *
 * +---------------------------------------------+ *
 * |  P1.02         |           WAKEUP           | *
 * +---------------------------------------------+ *
 **/
#define DW3000_CS_Pin   NRF_GPIO_PIN_MAP(0, 20)
#define DW3000_CLK_Pin  NRF_GPIO_PIN_MAP(0, 16)
#define DW3000_MOSI_Pin NRF_GPIO_PIN_MAP(0, 17)
#define DW3000_MISO_Pin NRF_GPIO_PIN_MAP(0, 23)
#define DW3000_RST_Pin  NRF_GPIO_PIN_MAP(0, 15)
#define DW3000_IRQ_Pin  NRF_GPIO_PIN_MAP(0, 25)
#define DW3000_WUP_Pin  NRF_GPIO_PIN_MAP(1, 2)
/*OTP Memory Addresses definition*/
#define DW3000_OPT_EUI_ADDR_HIGH   (0x00) // Bytes[3:0]：EUI(high)
#define DW3000_OPT_EUI_ADDR_LOW    (0x01) // Bytes[3:0]：EUI(low)
#define DW3000_TX_CH5_ADDRESS      (0x11) // Bytes[3:0]: Tx Power(PRF64,ch5)
#define DW3000_TX_CH9_ADDRESS      (0x13) // Bytes[3:0]: Tx Power(PRF64,ch9)
#define DW3000_REF_ADDRESS         (0x19) // Bytes[3:2]: Reserved, Byte[1]: Temperature, Byte[0]: Voltigae
#define DW3000_PDOA_CODE_PRF64     (0x1A) // Bytes[3:2]: PDoA Code(ch5), Bytes [1:0]: PDoA Code(ch9)
#define DW3000_PG_CNT_ADDRESS      (0x1B) // Bytes[3:2]: PG Count(PRF64,ch5), Bytes[1:0]: PG Count(PRF64,ch9)
#define DW3000_ANT_DLY_ADDRESS     (0x1C) // Bytes[3:2]: internal antenna delay(PRF64,ch5), Bytes[1:0]：internal antenna delay(PRF64,ch9)
#define DW3000_PG_DLY_TRIM_ADDRESS (0x1E) // Byte[3]: PG Delay(PRF64,ch9), Byte[2]：PG Delay(PRF64,ch9), Byte[1]: Reserved, Byte[0]: XTAL Trim
#define DW3000_OTP_REVISION        (0x1F) // Bytes[3:1]: Reserved, Byte[0]: OTP Resvision
#define DW3000_PLATFORM_ID         (0x5B) // Bytes[3:1]: Reserved, Byte[0]: Platform ID
#define DW3000_OTP_MAP_MAX         (0x7F) // 0x7F

#endif /* HAL_DW3000 */

#ifdef __cplusplus
}
#endif /* end of __cplusplus */
