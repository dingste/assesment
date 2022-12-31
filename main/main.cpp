#include "assesmentItems.hpp"
#include "sdkconfig.h"
#include <stdio.h>

extern "C" {
void app_main(void);
}

void app_main(void) {
  assesmentLed *onboardLed = new assesmentLed(48); // GPIO of onboard RGB-led

  while (1) {
    onboardLed->doBlinkFrequence(1000);
  }

  delete onboardLed;
}
