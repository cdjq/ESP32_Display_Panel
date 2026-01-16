#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following MIPI-DSI LCDs:
 *      - EK79007
 *      - HX8399
 *      - ILI9881C
 *      - JD9165, JD9365
 *      - ST7701, ST7703, ST7796, ST77922
 */
#define EXAMPLE_LCD_NAME                SIMPLE
#define EXAMPLE_LCD_WIDTH               (480)
#define EXAMPLE_LCD_HEIGHT              (800)
#define EXAMPLE_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                // or `ESP_PANEL_LCD_COLOR_BITS_RGB666` / `ESP_PANEL_LCD_COLOR_BITS_RGB888`
#define EXAMPLE_LCD_DSI_PHY_LDO_ID      (3)     // -1 if not used
#define EXAMPLE_LCD_DSI_LANE_NUM        (2)     // ESP32-P4 supports 1 or 2 lanes
#define EXAMPLE_LCD_DSI_LANE_RATE_MBPS  (1000)  /* Single lane bit rate, should consult the LCD supplier or check the
                                                 * LCD drive IC datasheet for the supported lane rate.
                                                 * ESP32-P4 supports max 1500Mbps
                                                 */
#define EXAMPLE_LCD_DPI_CLK_MHZ         (28)
#define EXAMPLE_LCD_DPI_COLOR_BITS      (EXAMPLE_LCD_COLOR_BITS)
#define EXAMPLE_LCD_DPI_TIMINGS_HPW     (2)
#define EXAMPLE_LCD_DPI_TIMINGS_HBP     (40)
#define EXAMPLE_LCD_DPI_TIMINGS_HFP     (40)
#define EXAMPLE_LCD_DPI_TIMINGS_VPW     (2)
#define EXAMPLE_LCD_DPI_TIMINGS_VBP     (10)
#define EXAMPLE_LCD_DPI_TIMINGS_VFP     (180)
#define EXAMPLE_LCD_USE_EXTERNAL_CMD    (1)
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
/**
 * @brief LCD vendor initialization commands
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * The initialization sequence can be specified in two formats:
 * 1. Raw format:
 *    {command, (uint8_t []){data0, data1, ...}, data_size, delay_ms}
 * 2. Helper macros:
 *    - ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, {data0, data1, ...})
 *    - ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */

const esp_panel_lcd_vendor_init_cmd_t lcd_init_cmd[] = {
        {0x11, (uint8_t []){0x00}, 0, 120},                   
        {0x99, (uint8_t []){0x71, 0x02, 0xa2}, 3, 0},        
        {0x99, (uint8_t []){0x71, 0x02, 0xa3}, 3, 0},        
        {0x99, (uint8_t []){0x71, 0x02, 0xa4}, 3, 0},        
        {0xA4, (uint8_t []){0x31}, 1, 0},                     
        {0xB0, (uint8_t []){0x22, 0x57, 0x1E, 0x61, 0x2F, 0x57, 0x61}, 7, 0}, 
        {0xB7, (uint8_t []){0x64, 0x64}, 2, 0},               
        {0xBF, (uint8_t []){0xB4, 0xB4}, 2, 0},               
        {0xC8, (uint8_t []){0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, 
                            0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, 
                            0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, 
                            0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 37, 0}, 
        {0xC9, (uint8_t []){0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, 
                            0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, 
                            0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, 
                            0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 37, 0}, 
        {0xD7, (uint8_t []){0x10, 0x0C, 0x36, 0x19, 0x90, 0x90}, 6, 0}, 
        {0xA3, (uint8_t []){0x51, 0x03, 0x80, 0xCF, 0x44, 0x00, 0x00, 0x00, 0x00, 
                            0x04, 0x78, 0x78, 0x00, 0x1A, 0x00, 0x45, 0x05, 0x00, 0x00, 
                            0x00, 0x00, 0x46, 0x00, 0x00, 0x02, 0x20, 0x52, 0x00, 0x05, 
                            0x00, 0x00, 0xFF}, 32, 0}, 
        {0xA6, (uint8_t []){0x02, 0x00, 0x24, 0x55, 0x35, 0x00, 0x38, 0x00, 0x78, 
                            0x78, 0x00, 0x24, 0x55, 0x36, 0x00, 0x37, 0x00, 0x78, 0x78, 
                            0x02, 0xAC, 0x51, 0x3A, 0x00, 0x00, 0x00, 0x78, 0x78, 0x03, 
                            0xAC, 0x21, 0x00, 0x04, 0x00, 0x00, 0x78, 0x78, 0x3e, 0x00, 
                            0x06, 0x00, 0x00, 0x00, 0x00}, 43, 0}, 
        {0xA7, (uint8_t []){0x19, 0x19, 0x00, 0x64, 0x40, 0x07, 0x16, 0x40, 0x00, 
                            0x04, 0x03, 0x78, 0x78, 0x00, 0x64, 0x40, 0x25, 0x34, 0x00, 
                            0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x64, 0x40, 0x4B, 0x5A, 
                            0x00, 0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x24, 0x40, 0x69, 
                            0x78, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x00, 0x44}, 48, 0}, 
        {0xAC, (uint8_t []){0x08, 0x0A, 0x11, 0x00, 0x13, 0x03, 0x1B, 0x18, 0x06, 
                            0x1A, 0x19, 0x1B, 0x1B, 0x1B, 0x18, 0x1B, 0x09, 0x0B, 0x10, 
                            0x02, 0x12, 0x01, 0x1B, 0x18, 0x06, 0x1A, 0x19, 0x1B, 0x1B, 
                            0x1B, 0x18, 0x1B, 0xFF, 0x67, 0xFF, 0x67, 0x00}, 37, 0}, 
        {0xAD, (uint8_t []){0xCC, 0x40, 0x46, 0x11, 0x04, 0x78, 0x78}, 7, 0}, 
        {0xE8, (uint8_t []){0x30, 0x07, 0x00, 0x94, 0x94, 0x9C, 0x00, 0xE2, 0x04, 
                           0x00, 0x00, 0x00, 0x00, 0xEF}, 14, 0}, 
        {0xE7, (uint8_t []){0x8B, 0x3C, 0x00 ,0x0C ,0xF0 ,0x5D, 0x00, 0x5D, 0x00, 
                            0x5D, 0x00, 0x5D, 0x00, 0xFF, 0x00, 0x08, 0x7B, 0x00, 0x00, 
                            0xC8, 0x6A, 0x5A, 0x08, 0x1A, 0x3C, 0x00, 0x81, 0x01, 0xCC, 
                            0x01, 0x7F, 0xF0, 0x22}, 33, 0}, 
        {0x11, (uint8_t []){0x00}, 0, 120},                     
        {0x29, (uint8_t []){0x00}, 0, 20},                       
        {0x35, (uint8_t []){0x00}, 1, 0},                       
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_RST_IO          (10)    // Set to -1 if not used
#define EXAMPLE_LCD_BL_IO           (11)    // Set to -1 if not used
#define EXAMPLE_LCD_BL_ON_LEVEL     (1)
#define EXAMPLE_LCD_BL_OFF_LEVEL    (!EXAMPLE_LCD_BL_ON_LEVEL)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_TOUCH_NAME              ST7123
#define EXAMPLE_TOUCH_ADDRESS           (0)
#define EXAMPLE_TOUCH_WIDTH             (EXAMPLE_LCD_WIDTH)
#define EXAMPLE_TOUCH_HEIGHT            (EXAMPLE_LCD_HEIGHT)
#define EXAMPLE_TOUCH_I2C_FREQ_HZ       (400 * 1000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_TOUCH_I2C_IO_SCL        (8)
#define EXAMPLE_TOUCH_I2C_IO_SDA        (7)
#define EXAMPLE_TOUCH_I2C_SCL_PULLUP    (0)
#define EXAMPLE_TOUCH_I2C_SDA_PULLUP    (0)
#define EXAMPLE_TOUCH_RST_IO            (-1)
#define EXAMPLE_TOUCH_RST_ACTIVE_LEVEL  (0)
#define EXAMPLE_TOUCH_INT_IO            (-1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please update the following configuration according to your test ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_TOUCH_ENABLE_CREATE_WITH_CONFIG (1)
#define EXAMPLE_TOUCH_ENABLE_INTERRUPT_CALLBACK (0)
#define EXAMPLE_TOUCH_READ_PERIOD_MS            (30)

