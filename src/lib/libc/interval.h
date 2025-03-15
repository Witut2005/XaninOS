#include <stdint.h>
#include <stdbool.h>

typedef uint8_t* address_t;

enum XANIN_INTERVAL {
  INTERVALS_MAX = 5,
  INTERVAL_CLEAR = 0,
  INTERVAL_IN_USE = 1,
  INTERVAL_CANT_INIT = 0xFFFFFFFF
};

typedef uint32_t interval_id;
typedef void (*interval_handler)(address_t* args);

struct IntervalEntry {
  bool is_in_use;
  interval_handler handler;
  address_t* arguments;
  float timeout;
  float start_time;
};

typedef struct IntervalEntry IntervalEntry;
extern IntervalEntry XaninIntervals[INTERVALS_MAX];

#ifdef __cplusplus
extern "C" {
#endif

  interval_id interval_set(interval_handler handler, float ms, address_t* args);
  void interval_clear(interval_id used_interval);
  void do_interval(interval_id interval);
  void all_intervals_clear(void);

#ifdef __cplusplus
}
#endif
