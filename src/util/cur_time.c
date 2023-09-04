#include <time.h>
#include <stdbool.h>

#include "cur_time.h"
#include "prettify_c.h"

double current_time_secs() {
  static clock_t Start;
  static bool IsInit = false;

  clock_t now = clock();

  if (not IsInit) {
    Start = now;
    IsInit = true;
  }

  return ((double)(now - Start)) / CLOCKS_PER_SEC;
}