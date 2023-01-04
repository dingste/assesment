#include "assesmentItems.hpp"
#include "driver/gpio.h"

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
  ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config,
                                           &internalBoardLed.led_strip));

  led_strip_clear(
      internalBoardLed.led_strip); // Set all LED off 
};

assesmentLed::~assesmentLed(){
       led_strip_clear(internalBoardLed.led_strip);
};

void assesmentLed::doBlinkFrequence(unsigned int newBlinkFrequence) {
  internalBoardLed.pp.freq = (newBlinkFrequence/internalBoardLed.pp.base);
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
  vTaskDelay(internalBoardLed.pp.freq / portTICK_PERIOD_MS);
};

void assesmentLed::doBlinkColor(unsigned int rgb){
         internalBoardLed.color.red = (rgb / 16);
        internalBoardLed.color.green = (rgb / 8);
        internalBoardLed.color.blue = (rgb / 4);
}

// -------------------

assesmentPoti::assesmentPoti(adc_unit_t adcUnit, adc_channel_t a, adc_channel_t b) {
  adc_oneshot_unit_init_cfg_t init_config1 = {
      .unit_id = adcUnit,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &poti.adc1_handle));

  adc_oneshot_chan_cfg_t config = {
      .atten = ADC_ATTEN_DB_0, // max 950mV
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(poti.adc1_handle, a, &config));
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(poti.adc1_handle, b, &config));
  poti.calibrated =
      do_adc_calibration(adcUnit, ADC_ATTEN_DB_0, &poti.adc1_cali_handle);
};

assesmentPoti::~assesmentPoti() {
  // Tear Down
  ESP_ERROR_CHECK(adc_oneshot_del_unit(poti.adc1_handle));

 #if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  if (poti.calibrated) {
    ESP_ERROR_CHECK(
        adc_cali_delete_scheme_curve_fitting(poti.adc1_cali_handle));
  }
#endif
};

bool assesmentPoti::do_adc_calibration(adc_unit_t unit, adc_atten_t atten,
                                       adc_cali_handle_t *out_handle) {
  adc_cali_handle_t handle = NULL;
  esp_err_t ret = ESP_FAIL;
  bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  if (!calibrated) {
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = unit,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
    if (ret == ESP_OK) {
      calibrated = true;
    }
  }
#endif
  *out_handle = handle;
  return calibrated;
};

bool assesmentPoti::onValueChange() {
  int currentValue = 0;
  ESP_ERROR_CHECK(
      adc_oneshot_read(poti.adc1_handle, ADC_CHANNEL_2, &currentValue));
  if (poti.calibrated) {
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(poti.adc1_cali_handle, currentValue,
                                            &poti.voltage));
  }
  if (currentValue>0 && currentValue<4095 && currentValue != poti.value) {
    poti.value = currentValue;
    return true;
  }
  return false;
};

const int &assesmentPoti::getValue() { return poti.value; };