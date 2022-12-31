/*
 * Only application on ESP32S3
 * getting app logic as lean as possible. (likely in C++)
 * Use items for assesment are declare in "assmentItems.h"
 *
 * Dieter Steuten
 */

#include "assesmentItems.hpp"
#include "sdkconfig.h"

extern "C" {
void app_main(void); // forward decleration of "C" Entrypoint
}

/* our only business logic , blink freq is mostly defined by assesmentLed init
 */
int invertPotiValue(int potiLevel) { return (4096 - potiLevel); }

/* main entrypoint
 * keep logic and dependencies with smallest and nice-readable lines as possible
*/
void app_main(void) {
  assesmentLed *onboardLed = new assesmentLed(48); // GPIO of onboard RGB-led
  assesmentPoti *potentiometer = new assesmentPoti(
      ADC_UNIT_1, ADC_CHANNEL_2, ADC_CHANNEL_3); // ADC1 on ESP32S3
  int potentiometerValue = 0;

  while (1) {
    if (potentiometer->onValueChange()) {
      potentiometerValue = potentiometer->getValue();
      onboardLed->doBlinkColor(potentiometerValue);
    }

    onboardLed->doBlinkFrequence(invertPotiValue(potentiometerValue));
  }

  // if while-loop will get some break-up criteria, beware of deleting :)
  delete onboardLed;
  delete potentiometer;
}
