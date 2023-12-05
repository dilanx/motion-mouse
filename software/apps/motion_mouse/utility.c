#include "utility.h"

int8_t utility_indexof(uint8_t* arr, uint8_t val, uint8_t len) {
  for (int i = 0; i < len; i++) {
    if (arr[i] == val) {
      return i;
    }
  }
  return -1;
}
