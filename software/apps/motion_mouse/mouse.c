#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "app_timer.h"
#include "utility.h"

#include "mouse.h"
#include "lsm303agr.h"
#include "amg88.h"
#include "da7280.h"

#define SWIPE_FRAME_COUNT 5
#define SENSITIVITY_X 0.35
#define SENSITIVITY_Y 0.35

APP_TIMER_DEF(motion_timer); // timer for other peripherals
APP_TIMER_DEF(select_timer); // infrared

static mouse_packet_t mouse_packet = {0};

// concentration of IR pixels per SWIPE_FRAME_COUNT frames
int8_t concentration[SWIPE_FRAME_COUNT] = {0};

static void calculate_velocity(float x_axis, float y_axis, float z_axis) {
  // Map x_axis, y_axis, and z_axis angles to velocities
  mouse_packet.vx = fabs(x_axis) > 20 ? -SENSITIVITY_X * x_axis : 0;
  mouse_packet.vy = fabs(z_axis) > 20 ? -SENSITIVITY_Y * z_axis : 0;
}

static void check_ir(ir_measurement_t measurement) {
  uint8_t count = 0;
  for (int i = 0; i < 64; i++) {
    if (measurement.pixels[i]) {
      count++;
      if (count > 32) {
        mouse_packet.click = true;
        return;
      }
    }
  }
  mouse_packet.click = false;
}

static void calculate_concentration(ir_measurement_t measurement) {
  uint8_t count_top = 0;
  uint8_t count_bottom = 0;

  for (int col = 0; col < 16; col++) {
    if (measurement.pixels[col]) {
      count_top++;
    }
  }

  for (int col = 48; col < 64; col++) {
    if (measurement.pixels[col]) {
      count_bottom++;
    }
  }

  for (int i = 0; i < SWIPE_FRAME_COUNT - 1; i++) {
    concentration[i] = concentration[i + 1];
  }

  if (count_top > 4 && count_top > count_bottom) {
    concentration[SWIPE_FRAME_COUNT - 1] = 1;
  } else if (count_bottom > 4 && count_bottom > count_top) {
    concentration[SWIPE_FRAME_COUNT - 1] = -1;
  } else {
    concentration[SWIPE_FRAME_COUNT - 1] = 0;
  }

  int8_t indn = utility_indexof(concentration, -1, SWIPE_FRAME_COUNT);
  int8_t indp = utility_indexof(concentration, 1, SWIPE_FRAME_COUNT);

  int8_t swipe_direction = 0;
  if (indn != -1 && indp != -1) {
    // larger magnitude means slower swipe
    swipe_direction = indp - indn;
  }

  if (swipe_direction > 0) {
    mouse_packet.scroll = 1;
  } else if (swipe_direction < 0) {
    mouse_packet.scroll = 2;
  } else {
    mouse_packet.scroll = 0;
  }
}

static void debug_print_concentration() {
  for (int i = 0; i < SWIPE_FRAME_COUNT; i++) {
    printf("%3d ", concentration[i]);
  }
  printf("\n");
}

static void print_packet() {
  printf(
    "%f,%f,%d,%d\n",
    mouse_packet.vx,
    mouse_packet.vy,
    mouse_packet.click,
    mouse_packet.scroll
  );
}

static void run_motion(void* _unused) {
  lsm303agr_measurement_t acc = lsm303agr_read_accelerometer();
  lsm303agr_measurement_t tilt = lsm303agr_calculate_tilt(acc);
  calculate_velocity(tilt.x_axis, tilt.y_axis, tilt.z_axis);

  print_packet();
}

static void run_select(void* _unused) {
  ir_measurement_t ir = amg88_read_pixels(25.0);
  calculate_concentration(ir);
  check_ir(ir);

  if (mouse_packet.click) {
    da7280_set_vibrate(125);
  } else {
    da7280_set_vibrate(0);
  }
}

/**
 * Initialize the mouse
 * 
 * Timers and drivers should be initialized beforehand
 */

void mouse_init(void) {
  lsm303agr_init();
  amg88_init();
  da7280_init();

  app_timer_create(&motion_timer, APP_TIMER_MODE_REPEATED, run_motion);
  app_timer_create(&select_timer, APP_TIMER_MODE_REPEATED, run_select);
  app_timer_start(motion_timer, 400, NULL);
  app_timer_start(select_timer, APP_TIMER_TICKS(100), NULL);
}
