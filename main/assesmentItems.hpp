#pragma once
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"

struct color_t {
  uint8_t red = 0;   // till 255
  uint8_t blue = 0;  // till 255
  uint8_t green = 0; // till 255
};

struct pulsePause_t {
  unsigned int freq = 1;
  static const unsigned int base = 100; // 1 of 100ms
};

struct led_t {
  unsigned int pin = 0;
  color_t color = color_t();
  pulsePause_t pp = pulsePause_t();
  uint8_t brightness = 0; // till 255
};

static   led_strip_handle_t led_strip;
static   uint8_t s_led_state = 0;

class assesmentLed {
public:
  assesmentLed(unsigned int pin) {
    internalBoardLed.pin = pin;
    led_strip_config_t strip_config = {
        .strip_gpio_num = internalBoardLed.pin,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    internalBoardLed.brightness = 10;
    internalBoardLed.color.red = 10;
    internalBoardLed.color.green = 10;
    internalBoardLed.color.blue = 255;

    led_strip_clear(led_strip); // Set all LED off to clear all pixels
  };
  ~assesmentLed(){};

  void doBlinkFrequence(unsigned int newBlinkFrequence) {
    internalBoardLed.pp.freq = newBlinkFrequence;
    if (s_led_state) {
      led_strip_set_pixel(
          led_strip, 0, 
          (internalBoardLed.color.red / internalBoardLed.brightness),
          (internalBoardLed.color.green / internalBoardLed.brightness),
          (internalBoardLed.color.blue / internalBoardLed.brightness)); 
      led_strip_refresh(led_strip);
    } else {
      led_strip_clear(led_strip);
    }
    s_led_state = !s_led_state;
    vTaskDelay(newBlinkFrequence / portTICK_PERIOD_MS);
  };

private:
  led_t internalBoardLed = led_t();


};