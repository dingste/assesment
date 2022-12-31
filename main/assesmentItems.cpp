#include "assesmentItems.hpp"

 assesmentLed::assesmentLed(unsigned int pin) {
        led_strip_config_t strip_config = {
        .strip_gpio_num = pin,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    internalBoardLed.brightness = 10; 
    internalBoardLed.color.red = 255;
    internalBoardLed.color.green = 255;
    internalBoardLed.color.blue = 10;
    ESP_ERROR_CHECK(
        led_strip_new_rmt_device(&strip_config, &rmt_config, &internalBoardLed.led_strip));

    led_strip_clear(internalBoardLed.led_strip); // Set all LED off to clear all pixels
  };
  assesmentLed::~assesmentLed(){};

  void assesmentLed::doBlinkFrequence(unsigned int newBlinkFrequence) {
    internalBoardLed.pp.freq = newBlinkFrequence;
    if (internalBoardLed.s_led_state && internalBoardLed.brightness > 0) {
      led_strip_set_pixel(
          internalBoardLed.led_strip, 0,
          (internalBoardLed.color.red / internalBoardLed.brightness),
          (internalBoardLed.color.green / internalBoardLed.brightness),
          (internalBoardLed.color.blue / internalBoardLed.brightness));
      led_strip_refresh(internalBoardLed.led_strip);
    } else {
      led_strip_clear(internalBoardLed.led_strip);
    }
    internalBoardLed.s_led_state = !internalBoardLed.s_led_state;
    vTaskDelay(newBlinkFrequence / portTICK_PERIOD_MS);
  };