#include <pebble.h>
    
GPath* aiming_path;

GBitmap * clockwiseBitmap;
GBitmap * counterClockwiseBitmap;
GBitmap * checkMarkBitmap;
ActionBarLayer * turnActionBar;

    
static const GPathInfo AIMING_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {100,0},
        {100,4},
        {0,4},
        {0,0}
    }
};

void aim_shot_course_layer_update_callback(Layer* layer, GContext* ctx);
void aim_shot_window_load(Window* window);
void aim_shot_window_unload(Window* window);