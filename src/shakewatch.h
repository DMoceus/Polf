#include <pebble.h>

void shake_watch_window_load(Window* window);
void shake_watch_window_unload(Window* window);
void shake_watch_accel_data_handler(AccelData* data, uint32_t num_samples);
void shake_watch_accel_data_service_provider(void* context);