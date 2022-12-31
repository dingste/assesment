/*
 * decleration of assesment items
 * so far:
 * 1x RGB Led (onboard DevKit)
 * 1x potentiometer
 * 
 * Cause of overviewable items/units for this project, both objects are declared (also implementation) in one file. 
 * Has benefit, to not struggeling with highly integrated dependencies of esp-idf. 
 * 
 * Dieter Steuten
 * 
*/

#pragma once
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "led_strip.h"
#include <string.h>

// --- about LED ---
struct color_t {
  uint8_t red = 0;   // till 255
  uint8_t blue = 0;  // till 255
  uint8_t green = 0; // till 255
};

struct pulsePause_t {
  unsigned int freq = 0;
  static const unsigned int base = 10; // 1 of 10ms
};

struct led_t {
  led_strip_handle_t led_strip = led_strip_handle_t();
  uint8_t s_led_state = 0; // could be bool
  color_t color = color_t();
  pulsePause_t pp = pulsePause_t();
  uint8_t brightness = 1; // 1..254 1=brightess
};

class assesmentLed {
public:
  assesmentLed(unsigned int pin);
  ~assesmentLed();

  void doBlinkFrequence(unsigned int newBlinkFrequence); // assign a new frequency of blinking
  void doBlinkColor(unsigned int rgb); // assign some color at our rgb led

private:
  led_t internalBoardLed = led_t(); // instance of dedicate led_t
};

// -----------------------------------

// --- about Potentiometer ---
struct poti_t {
  bool calibrated = false;
  int value = 0;   // 0..4095
  int voltage = 0; // 0..950mV
  adc_oneshot_unit_handle_t adc1_handle = adc_oneshot_unit_handle_t();
  adc_cali_handle_t adc1_cali_handle = adc_cali_handle_t();
};

class assesmentPoti {
public:
  assesmentPoti(adc_unit_t adcUnit, adc_channel_t a, adc_channel_t b);
  ~assesmentPoti();

  bool onValueChange(); // get indication someone turns the poti
  const int &getValue(); // return stored adc raw value

private:
  poti_t poti = poti_t(); // instance of dedicate poti_t
  bool do_adc_calibration(adc_unit_t unit, adc_atten_t atten,
                          adc_cali_handle_t *out_handle); // founded calibration routine for adc 
};