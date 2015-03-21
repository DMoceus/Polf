#include <pebble.h>
#include "mainmenu.h"
#include "about.h"

Window *main_menu_window;
MenuLayer* main_menu_layer;

Window* about_window;
TextLayer* about_layer;

//Main Menu Callbacks

void mm_draw_row_callback(GContext* ctx, Layer* cell_layer, MenuIndex* cell_index, void* callback_context){
    switch(cell_index->row){
        case 0:
            menu_cell_basic_draw(ctx,cell_layer,"Play Game","Level 1",NULL);
            break;
        case 1:
            menu_cell_basic_draw(ctx,cell_layer,"About","This App",NULL);
            break;
    }
}

uint16_t mm_num_rows_callback(MenuLayer* menu_layer, uint16_t section_index, void* callback_context){
    return 2;
}

void mm_select_click_callback(MenuLayer* menu_layer, MenuIndex* cell_index, void* callback_context){
    //int which = cell_index->row;
    switch(cell_index->row){
        case 0:
            break;
        case 1:
            about_window = window_create();
            WindowHandlers about_handlers = {
                .load   = about_window_load,
                .unload = about_window_unload
            };
            window_set_window_handlers(about_window,(WindowHandlers)about_handlers);
            window_stack_push(about_window,true);
            break;
    }
}


//LOADERS

void about_window_load(Window* window){
    about_layer = text_layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    text_layer_set_text(about_layer,"Pebble Golf\n\nTeam Members:\nDavid Morris\nJoseph Howard\nWeronika Kowalczyk");
    layer_add_child(window_get_root_layer(about_window),text_layer_get_layer(about_layer));
}

void about_window_unload(Window* window){
    text_layer_destroy(about_layer);
}

void mm_window_load(Window* window){
    main_menu_layer = menu_layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    menu_layer_set_click_config_onto_window(main_menu_layer,main_menu_window);
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback)mm_draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)mm_num_rows_callback,
        .select_click = (MenuLayerSelectCallback)mm_select_click_callback
    };
    menu_layer_set_callbacks(main_menu_layer,NULL,callbacks);
    layer_add_child(window_get_root_layer(main_menu_window),menu_layer_get_layer(main_menu_layer));
}

void mm_window_unload(Window* window){
    menu_layer_destroy(main_menu_layer);
}

void init(void) {
    main_menu_window = window_create();
    WindowHandlers mm_handlers = {
        .load = mm_window_load,
        .unload = mm_window_unload
    };
    window_set_window_handlers(main_menu_window,(WindowHandlers)mm_handlers);
    window_stack_push(main_menu_window,true);
}

void deinit(void) {
  window_destroy(main_menu_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
