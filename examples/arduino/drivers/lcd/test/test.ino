/**
 * Detailed usage of the example can be found in the [README.md](./README.md) file
 */

#include <Arduino.h>
#include <vector>
#include <esp_display_panel.hpp>
#include "test_conf.h"

using namespace esp_panel::drivers;

/* The following default configurations are for the board 'Espressif: ESP32-P4-Function-EV-Board, EK79007' */

#define _EXAMPLE_TOUCH_CLASS(name, ...) Touch##name(__VA_ARGS__)
#define EXAMPLE_TOUCH_CLASS(name, ...)  _EXAMPLE_TOUCH_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_TOUCH_ENABLE_INTERRUPT_CALLBACK
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

static Touch *create_touch_without_config(void)
{
    BusI2C *bus = new BusI2C(
        EXAMPLE_TOUCH_I2C_IO_SCL, EXAMPLE_TOUCH_I2C_IO_SDA,
#if EXAMPLE_TOUCH_ADDRESS == 0
        (BusI2C::ControlPanelFullConfig)ESP_PANEL_TOUCH_I2C_CONTROL_PANEL_CONFIG(EXAMPLE_TOUCH_NAME)
#else
        (BusI2C::ControlPanelFullConfig)ESP_PANEL_TOUCH_I2C_CONTROL_PANEL_CONFIG_WITH_ADDR(EXAMPLE_TOUCH_NAME, EXAMPLE_TOUCH_ADDRESS)
#endif
    );

    return new EXAMPLE_TOUCH_CLASS(
        EXAMPLE_TOUCH_NAME, bus, EXAMPLE_TOUCH_WIDTH, EXAMPLE_TOUCH_HEIGHT,
        EXAMPLE_TOUCH_RST_IO, EXAMPLE_TOUCH_INT_IO
    );
}

static Touch *create_touch_with_config(void)
{
    BusI2C::Config bus_config = {
        .host = BusI2C::HostPartialConfig{
            .sda_io_num = EXAMPLE_TOUCH_I2C_IO_SDA,
            .scl_io_num = EXAMPLE_TOUCH_I2C_IO_SCL,
            // .sda_pullup_en = EXAMPLE_TOUCH_I2C_SDA_PULLUP,
            // .scl_pullup_en = EXAMPLE_TOUCH_I2C_SCL_PULLUP,
            // .clk_speed = EXAMPLE_TOUCH_I2C_FREQ_HZ,
        },
        .control_panel = (BusI2C::ControlPanelFullConfig)
#if EXAMPLE_TOUCH_ADDRESS == 0
            ESP_PANEL_TOUCH_I2C_CONTROL_PANEL_CONFIG(EXAMPLE_TOUCH_NAME),
#else
            ESP_PANEL_TOUCH_I2C_CONTROL_PANEL_CONFIG_WITH_ADDR(EXAMPLE_TOUCH_NAME, EXAMPLE_TOUCH_ADDRESS),
#endif
    };
    Touch::Config touch_config = {
        .device = Touch::DevicePartialConfig{
            .x_max = EXAMPLE_TOUCH_WIDTH,
            .y_max = EXAMPLE_TOUCH_HEIGHT,
            .rst_gpio_num = EXAMPLE_TOUCH_RST_IO,
            .int_gpio_num = EXAMPLE_TOUCH_INT_IO,
            .levels_reset = EXAMPLE_TOUCH_RST_ACTIVE_LEVEL,
        },
    };

    return new EXAMPLE_TOUCH_CLASS(EXAMPLE_TOUCH_NAME, bus_config, touch_config);
}

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

static Touch *touch = nullptr;

void setup()
{
    Serial.begin(115200);

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

#if EXAMPLE_TOUCH_ENABLE_CREATE_WITH_CONFIG
    Serial.println("Initializing \"I2C\" touch with config");
    touch = create_touch_with_config();
#else
    Serial.println("Initializing \"I2C\" touch without config");
    touch = create_touch_without_config();
#endif

#if !EXAMPLE_TOUCH_ENABLE_CREATE_WITH_CONFIG
    /* Configure bus and touch before startup */
    auto bus = static_cast<BusI2C *>(touch->getBus());
    bus->configI2C_FreqHz(EXAMPLE_TOUCH_I2C_FREQ_HZ);
    bus->configI2C_PullupEnable(EXAMPLE_TOUCH_I2C_SDA_PULLUP, EXAMPLE_TOUCH_I2C_SCL_PULLUP);
    touch->configResetActiveLevel(EXAMPLE_TOUCH_RST_ACTIVE_LEVEL);
#endif

    assert(touch->begin());
#if EXAMPLE_TOUCH_ENABLE_INTERRUPT_CALLBACK
    if (touch->isInterruptEnabled()) {
        touch->attachInterruptCallback(onTouchInterruptCallback);
    }
#endif

    Serial.println("Reading touch points and buttons...");
}

void loop()
{
    touch->readRawData(-1, -1, EXAMPLE_TOUCH_READ_PERIOD_MS);

    std::vector<TouchPoint> points;
    int i = 0;
    touch->getPoints(points);
    for (auto &point : points) {
        Serial.printf("Touch point(%d): x %d, y %d, strength %d\n", i++, point.x, point.y, point.strength);
    }

    std::vector<TouchButton> buttons;
    i = 0;
    touch->getButtons(buttons);
    for (auto &button : buttons) {
        Serial.printf("Touch button(%d): %d\n", i++, button.second);
    }

    if (!touch->isInterruptEnabled()) {
        delay(EXAMPLE_TOUCH_READ_PERIOD_MS);
    }
}
