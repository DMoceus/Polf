#include <pebble.h>
    
GPath* hole_one;
GPath *hole_path;
GPath *green_path;
GPath *sand1_path;
GPath *sand2_path;
GPath *sand3_path;
GPath *teebox_path;
int currX = 80;
int currY = 23;
    
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

static const GPathInfo GREEN = {
  12,
  (GPoint []) {
    {80, 15},
    {76, 17},
    {72, 21},
    {70, 25},
    {72, 29},
    {76, 33},
    {80, 35},
    {84, 33},
    {88, 31},
    {90, 25},
    {88, 21},
    {84, 17}
    
  }
};

static const GPathInfo HOLE = {
  4,
  (GPoint []) {         
    {80, 23},
    {85, 23},
    {85, 20},
    {80, 20}
  }
};

static const GPathInfo SAND1 = {
  10,
  (GPoint []) {
    {67,20},
    {83, 15},
    
    {85, 14},
    {85, 11},
    
    {83, 10},
      
    {7, 6},
    {75, 6},
      
    {67,10}, 
    
    {65, 11},
    {65, 14}
  }
};
static const GPathInfo SAND2 = {
  10,
  (GPoint []) {
    {67,65},
    {93, 55},
    {95, 54},
    {95, 51},
    {93, 50},
    {81, 44},
    {79, 44},
    {67,50}, 
    {65, 51},
    {65, 64}
  }
};
  
  static const GPathInfo SAND3 = {
  4,
  (GPoint []) {
    {30,50},
    {30,80},
    {40,80},
    {40,50} 
  }
};

static const GPathInfo TEEBOX = {
  4,
  (GPoint []) {
    {60, 125},
    {60, 120},
    {70, 120},
    {70, 125} 
  }
};

void show_course_window_load(Window* window);
void show_course_window_unload(Window* window);
void show_course_select_click_handler(ClickRecognizerRef recognizer, void* context);
void show_course_config_provider(Window* window);