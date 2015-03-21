#include <pebble.h>
    
static const GPathInfo HOLE_ONE_FAIRWAY = {
  22,
  (GPoint []) {
    {60, 130},
    {43, 117},
    {30, 100},
    {23, 80},
    {20, 60},  
    {25, 30},
    {27, 25},
    {30, 20},
    {35, 10},
    {40, 6}, 
    {60, 0},
    {80, 3},
    {90, 10},
    {100, 20},
    {102, 35},
    {100, 50},
    {93, 60},
    {70, 70},
    {72, 90}, 
    {80, 115},
    {77, 125},
    {70, 130}
  }
};

void show_course_window_load(Window* window);
void show_course_window_unload(Window* window);
void show_course_select_click_handler(ClickRecognizerRef recognizer, void* context);
void show_course_config_provider(Window* window);