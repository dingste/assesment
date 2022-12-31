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
  led_strip_handle_t led_strip;
  uint8_t s_led_state = 0;
  color_t color = color_t();
  pulsePause_t pp = pulsePause_t();
  uint8_t brightness = 0; // 1..255 1=brightess
};

class assesmentLed {
public:
  assesmentLed(unsigned int pin);
  ~assesmentLed();
  void doBlinkFrequence(unsigned int newBlinkFrequence);

private:
  led_t internalBoardLed = led_t();
};