#include "nrf_delay.h"

#include "app_timer.h"
#include "i2c.h"
#include "mouse.h"

#include "lsm303agr.h"
#include "amg88.h"
#include "da7280.h"

int main(void) {
  printf("Motion mouse is here!\n");

  i2c_init();
  app_timer_init();
  mouse_init();

  while (1) {
    nrf_delay_ms(1000);
  }
}
