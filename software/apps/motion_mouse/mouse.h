#pragma once

typedef struct {
  float vx;
  float vy;
  bool click;
  uint8_t scroll; // 0 = no, 1 = up, 2 = down
} mouse_packet_t;

void mouse_init(void);
