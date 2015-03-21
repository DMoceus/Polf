#include <pebble.h>
void mm_draw_row_callback(GContext* ctx, Layer* cell_layer, MenuIndex* cell_index, void* callback_context);
uint16_t mm_num_rows_callback(MenuLayer* menu_layer, uint16_t section_index, void* callback_context);
void mm_select_click_callback(MenuLayer* menu_layer, MenuIndex* cell_index, void* callback_context);
void mm_window_load(Window* window);
void mm_window_unload(Window* window);