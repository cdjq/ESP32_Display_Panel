/**
 * Detailed usage of the example can be found in the [README.md](./README.md) file
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>

using namespace esp_panel::drivers;

/* The following default configurations are for the board 'Espressif: ESP32-P4-Function-EV-Board, EK79007' */
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
/////////////////////// Please update the following configuration according to your test ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_ENABLE_CREATE_WITH_CONFIG   (1)
#define EXAMPLE_LCD_ENABLE_PRINT_FPS            (1)
#define EXAMPLE_LCD_ENABLE_DRAW_FINISH_CALLBACK (1)
#define EXAMPLE_LCD_ENABLE_DSI_PATTERN_TEST     (1)

#define _EXAMPLE_LCD_CLASS(name, ...)   LCD_##name(__VA_ARGS__)
#define EXAMPLE_LCD_CLASS(name, ...)    _EXAMPLE_LCD_CLASS(name, ##__VA_ARGS__)

static LCD *create_lcd_without_config(void)
{
    BusDSI *bus = new BusDSI(
        /* DSI */
        EXAMPLE_LCD_DSI_LANE_NUM, EXAMPLE_LCD_DSI_LANE_RATE_MBPS,
        /* DPI */
        EXAMPLE_LCD_DPI_CLK_MHZ, EXAMPLE_LCD_DPI_COLOR_BITS, EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
        EXAMPLE_LCD_DPI_TIMINGS_HPW, EXAMPLE_LCD_DPI_TIMINGS_HBP, EXAMPLE_LCD_DPI_TIMINGS_HFP,
        EXAMPLE_LCD_DPI_TIMINGS_VPW, EXAMPLE_LCD_DPI_TIMINGS_VBP, EXAMPLE_LCD_DPI_TIMINGS_VFP,
        /* PHY LDO */
        EXAMPLE_LCD_DSI_PHY_LDO_ID
    );

    /**
     * Take `ILI9881C` as an example, the following is the actual code after macro expansion:
     *      LCD_ILI9881C(bus, 1024, 600, 16, 27);
     */
    return new EXAMPLE_LCD_CLASS(
        EXAMPLE_LCD_NAME, bus, EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_RST_IO
    );
}

static LCD *create_lcd_with_config(void)
{
    BusDSI::Config bus_config = {
        .host = BusDSI::HostPartialConfig{
            .num_data_lanes = EXAMPLE_LCD_DSI_LANE_NUM,
            .lane_bit_rate_mbps = EXAMPLE_LCD_DSI_LANE_RATE_MBPS,
        },
        .refresh_panel = BusDSI::RefreshPanelPartialConfig{
            .dpi_clock_freq_mhz = EXAMPLE_LCD_DPI_CLK_MHZ,
            .bits_per_pixel = EXAMPLE_LCD_DPI_COLOR_BITS,
            .h_size = EXAMPLE_LCD_WIDTH,
            .v_size = EXAMPLE_LCD_HEIGHT,
            .hsync_pulse_width = EXAMPLE_LCD_DPI_TIMINGS_HPW,
            .hsync_back_porch = EXAMPLE_LCD_DPI_TIMINGS_HBP,
            .hsync_front_porch = EXAMPLE_LCD_DPI_TIMINGS_HFP,
            .vsync_pulse_width = EXAMPLE_LCD_DPI_TIMINGS_VPW,
            .vsync_back_porch = EXAMPLE_LCD_DPI_TIMINGS_VBP,
            .vsync_front_porch = EXAMPLE_LCD_DPI_TIMINGS_VFP,
        },
        .phy_ldo = BusDSI::PHY_LDO_PartialConfig{
            .chan_id = EXAMPLE_LCD_DSI_PHY_LDO_ID,
        },
    };
    LCD::Config lcd_config = {
        .device = LCD::DevicePartialConfig{
            .reset_gpio_num = EXAMPLE_LCD_RST_IO,
            .bits_per_pixel = EXAMPLE_LCD_COLOR_BITS,
        },
        .vendor = LCD::VendorPartialConfig{
            .hor_res = EXAMPLE_LCD_WIDTH,
            .ver_res = EXAMPLE_LCD_HEIGHT,
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
            .init_cmds = lcd_init_cmd,
            .init_cmds_size = sizeof(lcd_init_cmd) / sizeof(lcd_init_cmd[0]),
#endif
        },
    };

    /**
     * Take `ILI9881C` as an example, the following is the actual code after macro expansion:
     *      LCD_ILI9881C(bus_config, lcd_config);
     */
    return new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, bus_config, lcd_config);
}

#if EXAMPLE_LCD_ENABLE_PRINT_FPS
#define EXAMPLE_LCD_PRINT_FPS_PERIOD_MS         (1000)
#define EXAMPLE_LCD_PRINT_FPS_COUNT_MAX         (50)

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onLCD_RefreshFinishCallback(void *user_data)
{
    if (start_time == 0) {
        start_time = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= EXAMPLE_LCD_PRINT_FPS_COUNT_MAX) {
        fps = EXAMPLE_LCD_PRINT_FPS_COUNT_MAX * 1000 / (millis() - start_time);
        esp_rom_printf("LCD FPS: %d\n", fps);
        frame_count = 0;
        start_time = millis();
    }

    return false;
}
#endif // EXAMPLE_LCD_ENABLE_PRINT_FPS

#if EXAMPLE_LCD_ENABLE_DRAW_FINISH_CALLBACK
IRAM_ATTR bool onLCD_DrawFinishCallback(void *user_data)
{
    esp_rom_printf("LCD draw finish callback\n");

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    // pinMode(10, OUTPUT);
    
    // digitalWrite(10, 0);
    // delay(10);
    // digitalWrite(10, 1);
    // delay(100);

#if EXAMPLE_LCD_BL_IO >= 0
    pinMode(11, OUTPUT);
    digitalWrite(11, 1);

    Serial.println("Backlight  in this test");
#endif

#if EXAMPLE_LCD_ENABLE_CREATE_WITH_CONFIG
    Serial.println("Initializing \"MIPI-DSI\" LCD with config");
    auto lcd = create_lcd_with_config();
#else
    Serial.println("Initializing \"MIPI-DSI\" LCD without config");
    auto lcd = create_lcd_without_config();
#endif

    /* Configure bus and lcd before startup */
#if EXAMPLE_LCD_USE_EXTERNAL_CMD && !EXAMPLE_LCD_ENABLE_CREATE_WITH_CONFIG
    // Configure external LCD initialization commands
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd)/sizeof(lcd_init_cmd[0]));
#endif
#if EXAMPLE_LCD_ENABLE_DRAW_FINISH_CALLBACK
    // Attach a callback function which will be called when every bitmap drawing is completed
    lcd->attachDrawBitmapFinishCallback(onLCD_DrawFinishCallback);
#endif
#if EXAMPLE_LCD_ENABLE_PRINT_FPS
    // Attach a callback function which will be called when the Vsync signal is detected
    lcd->attachRefreshFinishCallback(onLCD_RefreshFinishCallback);
#endif

    /* Startup the LCD and operate it */
    assert(lcd->begin());
    if (lcd->getBasicAttributes().basic_bus_spec.isFunctionValid(LCD::BasicBusSpecification::FUNC_DISPLAY_ON_OFF)) {
        lcd->setDisplayOnOff(true);
    }

#if EXAMPLE_LCD_ENABLE_DSI_PATTERN_TEST
    Serial.println("Show MIPI-DSI color bar patterns");
    lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BAR_HORIZONTAL);
    delay(1000);
    lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BAR_VERTICAL);
    delay(1000);
    lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BER_VERTICAL);
    delay(1000);
    lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::NONE);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    lcd->colorBarTest();
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
