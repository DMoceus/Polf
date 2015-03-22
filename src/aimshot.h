#include <pebble.h>
    
GPath* aiming_path;
GPath* putter_aim;
GPath* iron_three_aim;
GPath* iron_five_aim;
GPath* iron_seven_aim;
GPath* iron_nine_aim;

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
static const GPathInfo PUTTER_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {10,0},
        {10,4},
        {0,4},
        {0,0}
    }
};
static const GPathInfo THREE_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {100,0},
        {100,4},
        {0,4},
        {0,0}
    }
};
static const GPathInfo FIVE_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {70,0},
        {70,4},
        {0,4},
        {0,0}
    }
};
static const GPathInfo SEVEN_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {50,0},
        {50,4},
        {0,4},
        {0,0}
    }
};
static const GPathInfo NINE_PATH_INFO = {
    .num_points = 5,
    .points = (GPoint[]){
        {0,0},
        {30,0},
        {30,4},
        {0,4},
        {0,0}
    }
};

void aim_shot_course_layer_update_callback(Layer* layer, GContext* ctx);
void aim_shot_window_load(Window* window);
void aim_shot_window_unload(Window* window);