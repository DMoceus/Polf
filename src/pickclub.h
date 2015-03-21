#include <pebble.h>

#define PUTTER_LOW 1
#define PUTTER_HIGH 10
#define IRON_NINE_LOW 10
#define IRON_NINE_HIGH 30
#define IRON_SEVEN_LOW 30
#define IRON_SEVEN_HIGH 50
#define IRON_FIVE_LOW 50
#define IRON_FIVE_HIGH 70
#define IRON_THREE_LOW 70
#define IRON_THREE_HIGH 100
    
const int PUTTER = 0;
const int IRON_NINE = 1;
const int IRON_SEVEN = 2;
const int IRON_FIVE = 3;
const int IRON_THREE = 4;

void pc_draw_row_callback(GContext* ctx, Layer* cell_layer, MenuIndex* cell_index, void* callback_context);
uint16_t pc_num_rows_callback(MenuLayer* menu_layer, uint16_t section_index, void* callback_context);
void pc_select_click_callback(MenuLayer* menu_layer, MenuIndex* cell_index, void* callback_context);
void pick_club_window_load(Window* window);
void pick_club_window_unload(Window* window);