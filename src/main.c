#include <pebble.h>
#include "mainmenu.h"
#include "about.h"
#include "showcourse.h"
#include "pickclub.h"

Window* main_menu_window;
MenuLayer* main_menu_layer;

Window* about_window;
TextLayer* about_layer;

Window* show_course_window;
Layer* show_course_layer;
GPath* hole_one;

Window* pick_club_window;
MenuLayer* pick_club_layer;
int club_choice;
int hit_low_range;
int hit_high_range;


//Show Course Callbacks

void show_course_layer_update_callback(Layer* layer, GContext* ctx){
    graphics_context_set_fill_color(ctx,GColorJaegerGreen);
    gpath_draw_filled(ctx,hole_one);
}

//Pick Club Callbacks

void pc_draw_row_callback(GContext* ctx, Layer* cell_layer, MenuIndex* cell_index, void* callback_context){
    switch(cell_index->row){
        case 0:
            menu_cell_basic_draw(ctx,cell_layer,"Putter","Range: 1-10",NULL);
            break;
        case 1:
            menu_cell_basic_draw(ctx,cell_layer,"Nine Iron","Range: 10-30",NULL);
            break;
        case 2:
            menu_cell_basic_draw(ctx,cell_layer,"Seven Iron","Range: 30-50",NULL);
            break;
        case 3:
            menu_cell_basic_draw(ctx,cell_layer,"Five Iron","Range: 50-70",NULL);
            break;
        case 4:
            menu_cell_basic_draw(ctx,cell_layer,"Three Iron","Range: 70-100",NULL);
            break;
    }
}
uint16_t pc_num_rows_callback(MenuLayer* menu_layer, uint16_t section_index, void* callback_context){
    return 5;
}

void pc_select_click_callback(MenuLayer* menu_layer, MenuIndex* cell_index, void* callback_context){
        //TODO: Link To Next State Once Next State is Made
    switch(cell_index->row){
        case 0:
            club_choice = PUTTER;
            hit_low_range = PUTTER_LOW;
            hit_high_range = PUTTER_HIGH;
            break;
        case 1:
            club_choice = IRON_NINE;
            hit_low_range = IRON_NINE_LOW;
            hit_high_range = IRON_NINE_HIGH;
            break;
        case 2:
            club_choice = IRON_SEVEN;
            hit_low_range = IRON_SEVEN_LOW;
            hit_high_range = IRON_SEVEN_HIGH;
            break;
        case 3:
            club_choice = IRON_FIVE;
            hit_low_range = IRON_FIVE_LOW;
            hit_high_range = IRON_FIVE_HIGH;
            break;
        case 4:
            club_choice = IRON_THREE;
            hit_low_range = IRON_THREE_LOW;
            hit_high_range = IRON_THREE_HIGH;
            break;
    }
}

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
            show_course_window = window_create();
            hole_one = gpath_create(&HOLE_ONE_FAIRWAY);
            window_set_background_color(show_course_window, GColorBlack);
            window_set_window_handlers(show_course_window, (WindowHandlers){
                .load   = show_course_window_load,
                .unload = show_course_window_unload
            });
            window_stack_push(show_course_window,true);
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

//Show Course Click Handlers

void show_course_select_click_handler(ClickRecognizerRef recognizer, void* context){
    
    //static const uint32_t const segments[] = {200,100,400};
}

void show_course_config_provider(Window* window){
    //window_single_click_subscribe
}

//LOADERS

void pick_club_window_load(Window* window){
    pick_club_layer = menu_layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    MenuLayerCallbacks callbacks = {
        .draw_row     = (MenuLayerDrawRowCallback)pc_draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)pc_num_rows_callback,
        .select_click = (MenuLayerSelectCallback)pc_select_click_callback
    };
}

void pick_club_window_unload(Window* window){
    menu_layer_destroy(pick_club_layer);
}

void show_course_window_load(Window* window){
    show_course_layer = layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    layer_set_update_proc(show_course_layer,show_course_layer_update_callback);
    layer_add_child(window_get_root_layer(show_course_window),show_course_layer);
}

void show_course_window_unload(Window* window){
    gpath_destroy(hole_one);
    layer_destroy(show_course_layer);
}

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
