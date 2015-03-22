#include <pebble.h>
#include "mainmenu.h"
#include "about.h"
#include "showcourse.h"
#include "pickclub.h"
#include "calculations.h"
#include "aimshot.h"
#include "shakewatch.h"
#include "swinganimation.h"

static Window *s_victory_window;
static TextLayer *s_text_victory;
static TextLayer *s_text_score;
static TextLayer *s_text_par;
static Layer *s_canvas_layer;
static InverterLayer *s_inverted_layer;
static char buffer[15];
static char buff[15];
    
Window* main_menu_window;
MenuLayer* main_menu_layer;

Window* about_window;
TextLayer* about_layer;

Window* show_course_window;
Layer* show_course_layer;

Window* pick_club_window;
MenuLayer* pick_club_layer;

Window* aim_shot_window;
Layer* aim_shot_course_layer;
RotBitmapLayer* aim_shot_overlay_layer;
GBitmap* bitMap;

Window* shake_watch_window;
TextLayer* shake_watch_layer;

Window* swing_animation_window;
Layer* swing_animation_layer;
PropertyAnimation* anime;

bool checkAccel = true;

int club_choice;
int hit_low_range;
int hit_high_range;
int avg_hit;
int inTheRough;

int globalAngle;
int quadrant;
int actualAngle;

int currentX = 65;
int currentY = 123;
int nextX;
int nextY;

int holeMinX = 80;
int holeMaxX = 85;
int holeMinY = 20;
int holeMaxY = 23;

int score = 0;

bool sand = false;



//Calculations

bool victoryCondition(){
  if(
    (nextX <= holeMaxX && nextX >= holeMinX) 
    &&
    (nextY <= holeMaxY && nextY >= holeMinY)
  ){
    return true;
  }else if(club_choice == PUTTER){
      if(
            (
                (currentX <= holeMinX && nextX >= holeMaxX)
                ||
                (currentX >= holeMaxX && nextX <= holeMinX)
            )&&(
                (currentY <= holeMinY && nextY >= holeMaxY)
                ||
                (currentY >= holeMaxY && nextY <= holeMaxY)
            )
          
      ){
          return true;
      }else{
          return false;
      }
  }else{
    return false;
  }
}

void initializeAngle(){
    globalAngle = 270;
    quadrant = 2;
    actualAngle = 90;
}

void rotateRight(){
    globalAngle += 6;
    if(360 <= globalAngle){
        globalAngle = globalAngle - 360;
    }
    if(0 <= globalAngle && globalAngle <= 90){
        quadrant = 4;
        actualAngle = globalAngle;
    } else if(90 < globalAngle && globalAngle <= 180){
        quadrant = 3;
        actualAngle = 180 - globalAngle;
    } else if(180 < globalAngle && globalAngle <= 270){
        quadrant = 1;
        actualAngle = globalAngle - 180;
    } else{
        quadrant = 2;
        actualAngle = 360 - globalAngle;
    }
}
void rotateLeft(){
    if(globalAngle < 6){
        globalAngle = globalAngle + 360;
    }
    globalAngle = globalAngle - 6;
    if(0 <= globalAngle && globalAngle <= 90){
        quadrant = 4;
        actualAngle = globalAngle;
    } else if(90 < globalAngle && globalAngle <= 180){
        quadrant = 3;
        actualAngle = 180 - globalAngle;
    } else if(180 < globalAngle && globalAngle <= 270){
        quadrant = 1;
        actualAngle = globalAngle - 180;
    } else{
        quadrant = 2;
        actualAngle = 360 - globalAngle;
    }
}

int roll_power(){
    //praise be to RNGesus
    int delta = hit_high_range - hit_low_range;
    srand(time(NULL));
    int show_me_7 = (rand() % delta)+hit_low_range;
    if(sand){
        if(show_me_7 >= avg_hit){
            srand(time(NULL));
            show_me_7 = (rand() % delta)+hit_low_range;
        }
    }
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", show_me_7);
    return show_me_7;
}
int calcYCoord(int angleIn, int power){
    angleIn = angleIn%360;
    int angle = (angleIn*TRIG_MAX_ANGLE)/360;
    int32_t ratio= ((sin_lookup(angle))*1000)/TRIG_MAX_RATIO;
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%ld", ((ratio*power)+500)/1000);
    return ((ratio*5)+500)/1000;
}
int calcXCoord(int angleIn, int power){
    angleIn = angleIn%360;
    int angle = (angleIn*TRIG_MAX_ANGLE)/360;
    int32_t ratio= ((cos_lookup(angle))*1000)/TRIG_MAX_RATIO;
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%ld", ((ratio*power)+500)/1000);
    return ((ratio*power)+500)/1000;
}

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {

  GPoint p0 = GPoint(0, 97);
  GPoint p1 = GPoint(144, 97);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p0, p1);
	
	GPoint p2 = GPoint(0, 67);
  GPoint p3 = GPoint(144, 67);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p2, p3);
	
	GPoint p4 = GPoint(0, 127);
  GPoint p5 = GPoint(144, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p4, p5);
	
	GPoint p18 = GPoint(0, 67);
  GPoint p19 = GPoint(0, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p18, p19);
	
	GPoint p6 = GPoint(43, 67);
  GPoint p7 = GPoint(43, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p6, p7);

	GPoint p8 = GPoint(63, 67);
  GPoint p9 = GPoint(63, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p8, p9);
	
	GPoint p10 = GPoint(83, 67);
  GPoint p11 = GPoint(83, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p10, p11);
	
	GPoint p12 = GPoint(103, 67);
  GPoint p13 = GPoint(103, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p12, p13);
	
	GPoint p14 = GPoint(123, 67);
  GPoint p15 = GPoint(123, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p14, p15);
	
	GPoint p16 = GPoint(143, 67);
  GPoint p17 = GPoint(143, 127);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p16, p17);
}

//Swing Animation Callbacks
void swing_animation_layer_update_callback(Layer* layer, GContext* ctx){
    graphics_context_set_fill_color(ctx, GColorJaegerGreen);
    gpath_draw_filled(ctx, hole_one);
    graphics_context_set_fill_color(ctx, GColorDarkGreen);
    gpath_draw_filled(ctx, green_path);
    graphics_context_set_fill_color(ctx, GColorWhite);
    gpath_draw_filled(ctx, hole_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand1_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand2_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand3_path);
    graphics_context_set_fill_color(ctx, GColorDarkGray);
    gpath_draw_filled(ctx, teebox_path);
    
}

void swing_animation_animator(){
    GRect from_frame = GRect(currX,currY,1,1);
    GRect to_frame   = GRect(nextX,nextY,1,1);
    anime = property_animation_create_layer_frame(swing_animation_layer, &from_frame, &to_frame);
    animation_schedule((Animation*)anime);
}

//Aim Shot Callbacks

void aim_shot_course_layer_update_callback(Layer* layer, GContext* ctx){
    graphics_context_set_fill_color(ctx, GColorJaegerGreen);
    gpath_draw_filled(ctx, hole_one);
    graphics_context_set_fill_color(ctx, GColorDarkGreen);
    gpath_draw_filled(ctx, green_path);
    graphics_context_set_fill_color(ctx, GColorWhite);
    gpath_draw_filled(ctx, hole_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand1_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand2_path);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    gpath_draw_filled(ctx, sand3_path);
    graphics_context_set_fill_color(ctx, GColorDarkGray);
    gpath_draw_filled(ctx, teebox_path);
    graphics_context_set_fill_color(ctx, GColorBlack);
    if(club_choice == PUTTER){
      aiming_path = putter_aim;
    }else if(club_choice == IRON_NINE){
      aiming_path = iron_nine_aim;
    }else if(club_choice == IRON_SEVEN){
      aiming_path = iron_seven_aim;
    }else if(club_choice == IRON_FIVE){
      aiming_path = iron_five_aim;
    }else if(club_choice == IRON_THREE){
      aiming_path = iron_three_aim;
    }
    gpath_draw_filled(ctx, aiming_path);
    gpath_rotate_to(aiming_path, TRIG_MAX_ANGLE / 360 * globalAngle);
    //rot_bitmap_set_compositing_mode(aim_shot_overlay_layer, GCompOpSet);
}

//Show Course Callbacks

void show_course_layer_update_callback(Layer* layer, GContext* ctx){
    graphics_context_set_fill_color(ctx,GColorJaegerGreen);
    gpath_draw_filled(ctx,hole_one);
    graphics_context_set_fill_color(ctx,GColorDarkGreen);
    gpath_draw_filled(ctx,green_path);
    graphics_context_set_fill_color(ctx,GColorWhite);
    gpath_draw_filled(ctx,hole_path);
    graphics_context_set_fill_color(ctx,GColorChromeYellow);
    gpath_draw_filled(ctx,sand1_path);
    graphics_context_set_fill_color(ctx,GColorChromeYellow);
    gpath_draw_filled(ctx,sand2_path);
    graphics_context_set_fill_color(ctx,GColorChromeYellow);
    gpath_draw_filled(ctx,sand3_path);
    graphics_context_set_fill_color(ctx,GColorDarkGray);
    gpath_draw_filled(ctx,teebox_path);
    //graphics_context_set_fill_color(ctx,GColorWhite);
    //gpath_draw_filled(ctx,ball_path);
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
    switch(cell_index->row){
        case 0:
            club_choice = PUTTER;
            avg_hit = PUTTER_AVG;
            hit_low_range = PUTTER_LOW;
            hit_high_range = PUTTER_HIGH;
            //bitMap = gbitmap_create_with_resource(RESOURCE_ID_OVERLAY_PUTTER);
            break;
        case 1:
            club_choice = IRON_NINE;
            avg_hit = IRON_NINE_AVG;
            hit_low_range = IRON_NINE_LOW;
            hit_high_range = IRON_NINE_HIGH;
            //bitMap = gbitmap_create_with_resource(RESOURCE_ID_OVERLAY_IRON_NINE);
            break;
        case 2:
            club_choice = IRON_SEVEN;
            avg_hit = IRON_SEVEN_AVG;
            hit_low_range = IRON_SEVEN_LOW;
            hit_high_range = IRON_SEVEN_HIGH;
            //bitMap = gbitmap_create_with_resource(RESOURCE_ID_OVERLAY_IRON_SEVEN);
            break;
        case 3:
            club_choice = IRON_FIVE;
            avg_hit = IRON_FIVE_AVG;
            hit_low_range = IRON_FIVE_LOW;
            hit_high_range = IRON_FIVE_HIGH;
            //bitMap = gbitmap_create_with_resource(RESOURCE_ID_OVERLAY_IRON_FIVE);
            break;
        case 4:
            club_choice = IRON_THREE;
            avg_hit = IRON_THREE_AVG;
            hit_low_range = IRON_THREE_LOW;
            hit_high_range = IRON_THREE_HIGH;
            //bitMap = gbitmap_create_with_resource(RESOURCE_ID_OVERLAY_IRON_THREE);
            break;
    }
    aim_shot_window = window_create();
    //aim_shot_overlay_layer = rot_bitmap_layer_create(bitMap);
    window_set_background_color(aim_shot_window, GColorArmyGreen);
    window_set_window_handlers(aim_shot_window, (WindowHandlers){
        .load   = aim_shot_window_load,
        .unload = aim_shot_window_unload
    });
    window_stack_push(aim_shot_window,true);
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
            green_path = gpath_create(&GREEN);
            hole_path = gpath_create(&HOLE);
            sand1_path = gpath_create(&SAND1);
            sand2_path = gpath_create(&SAND2);
            sand3_path = gpath_create(&SAND3);
            teebox_path = gpath_create(&TEEBOX);
            window_set_background_color(show_course_window, GColorArmyGreen);
            window_set_window_handlers(show_course_window, (WindowHandlers){
                .load   = show_course_window_load,
                .unload = show_course_window_unload,
                //.appear = show_course_window_appear
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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Select Handler");
    pick_club_window = window_create();
    WindowHandlers pc_handlers = {
        .load   = pick_club_window_load,
        .unload = pick_club_window_unload
    };
    window_set_window_handlers(pick_club_window,(WindowHandlers)pc_handlers);
    window_stack_push(pick_club_window,true);
}

void show_course_config_provider(Window* window){
    window_single_click_subscribe(BUTTON_ID_SELECT, show_course_select_click_handler);
}

//Aim Shot Click Handlers
void aim_shot_up_click_handler(ClickRecognizerRef recognizer, void* context){
    rotateLeft();
    layer_mark_dirty(aim_shot_course_layer);
}

void aim_shot_down_click_handler(ClickRecognizerRef recognizer, void* context){
    rotateRight();
    layer_mark_dirty(aim_shot_course_layer);
}

void aim_shot_select_click_handler(ClickRecognizerRef recognizer, void* context){
    shake_watch_window = window_create();
    WindowHandlers sw_handlers = {
        .load   = shake_watch_window_load,
        .unload = shake_watch_window_unload
    };
    window_set_window_handlers(shake_watch_window,(WindowHandlers)sw_handlers);
    window_stack_push(shake_watch_window,true);
}

void aim_shot_click_config_provider(void* context){
    window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)aim_shot_up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)aim_shot_down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)aim_shot_select_click_handler);
}

//Shake Watch Click Handlers

void shake_watch_accel_data_handler(AccelData* data, uint32_t num_samples){
    uint32_t i;
    if(checkAccel == true){
        for(i=0;i<num_samples;i++){
            if(data[i].x <  -800 ||
                data[i].x >  800 ||
                data[i].y < -800 ||
                data[i].y >  800 ||
                data[i].z < -1800 ||
                data[i].z >  -200){
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Accelerometer Found!");
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", data[i].x);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", data[i].y);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", data[i].z);
                checkAccel = false;
                switch(quadrant){
                    case 1:
                        nextX = currX - calcXCoord(actualAngle,roll_power());
                        nextY = currY + calcYCoord(actualAngle,roll_power());
                        break;
                    case 2:
                        nextX = currX + calcXCoord(actualAngle,roll_power());
                        nextY = currY + calcYCoord(actualAngle,roll_power());
                        break;
                    case 3:
                        nextX = currX - calcXCoord(actualAngle,roll_power());
                        nextY = currY - calcYCoord(actualAngle,roll_power());
                        break;
                    case 4:
                        nextX = currX + calcXCoord(actualAngle,roll_power());
                        nextY = currY - calcYCoord(actualAngle,roll_power());
                        break;
                }
                //Call Next Class
                swing_animation_window = window_create();
                WindowHandlers sa_handlers = {
                    .load   = swing_animation_window_load,
                    .unload = swing_animation_window_unload
                };
                window_set_window_handlers(swing_animation_window,(WindowHandlers)sa_handlers);
                window_stack_push(swing_animation_window,true);
                
                break;
            }
        }
    }
}

//LOADERS

void victory_window_load(Window* window){
    	//Create the text layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
	
	//INCREMENT THE SCORE

  s_text_victory = text_layer_create(GRect(0, 20, bounds.size.w, bounds.size.h-25 /* height */));
  text_layer_set_text(s_text_victory, "Victory!");
  text_layer_set_font(s_text_victory, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_text_victory, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_victory));
	
	char *parText = "Par       ";

	s_text_par = text_layer_create(GRect(5, 70, bounds.size.w, bounds.size.h-70));
	//text_layer_set_text(s_text_score, "Par: ");
	text_layer_set_font(s_text_par, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_text_par, GTextAlignmentLeft);
	layer_add_child(window_layer, text_layer_get_layer(s_text_par));
	snprintf(buff, 15, "%s %d", parText, score);
	text_layer_set_text(s_text_par, buff);	

	 
	char *scoreText = "Score  ";

	s_text_score = text_layer_create(GRect(5, 100, bounds.size.w, bounds.size.h-100));
	//text_layer_set_text(s_text_score, "Par: ");
	text_layer_set_font(s_text_score, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_text_score, GTextAlignmentLeft);
	layer_add_child(window_layer, text_layer_get_layer(s_text_score));
	snprintf(buffer, 15, "%s %d", scoreText, score);
	text_layer_set_text(s_text_score, buffer);
	
  s_canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, s_canvas_layer);

  // Set the update_proc
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
	
	//Create InverterLayer
	s_inverted_layer = inverter_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	layer_add_child(window_layer, inverter_layer_get_layer(s_inverted_layer));
	vibes_double_pulse();
    
    window_stack_remove(pick_club_window,false);
    window_stack_remove(aim_shot_window,false);
    window_stack_remove(shake_watch_window,false);
    window_stack_remove(swing_animation_window,false);
    window_stack_remove(show_course_window,false);
    
}

void victory_window_unload(Window* window){
    text_layer_destroy(s_text_victory);
    layer_destroy(s_canvas_layer);
    inverter_layer_destroy(s_inverted_layer);
}
//void show_course_config_provider(Window* window)

void swing_animation_window_load(Window* window){
    score++;
    swing_animation_layer = layer_create(GRect(0,0,144,168-16));
    layer_set_update_proc(swing_animation_layer,swing_animation_layer_update_callback);
    layer_add_child(window_get_root_layer(swing_animation_window),swing_animation_layer);
    if(victoryCondition()){
        //GOTO Victory
        s_victory_window = window_create();
        window_set_window_handlers(s_victory_window, (WindowHandlers) {
        .load = victory_window_load,
        .unload = victory_window_unload,
        });
        window_stack_push(s_victory_window, true);
    }
    else{
        //GOTO SHOW_COURSE
        currX = nextX;
        currY = nextY;
        
            /*hole_one = gpath_create(&HOLE_ONE_FAIRWAY);
            green_path = gpath_create(&GREEN);
            hole_path = gpath_create(&HOLE);
            sand1_path = gpath_create(&SAND1);
            sand2_path = gpath_create(&SAND2);
            sand3_path = gpath_create(&SAND3);
            teebox_path = gpath_create(&TEEBOX);*/
            window_stack_remove(show_course_window,false);
            show_course_window = window_create();
            hole_one = gpath_create(&HOLE_ONE_FAIRWAY);
            green_path = gpath_create(&GREEN);
            hole_path = gpath_create(&HOLE);
            sand1_path = gpath_create(&SAND1);
            sand2_path = gpath_create(&SAND2);
            sand3_path = gpath_create(&SAND3);
            teebox_path = gpath_create(&TEEBOX);
            window_set_background_color(show_course_window, GColorArmyGreen);
            window_set_window_handlers(show_course_window, (WindowHandlers){
                .load   = show_course_window_load,
                .unload = show_course_window_unload,
                //.appear = show_course_window_appear
            });
            window_stack_push(show_course_window,true);
        /*window_stack_remove(pick_club_window,false);
        window_stack_remove(aim_shot_window,false);
        window_stack_remove(shake_watch_window,false);
        layer_mark_dirty(show_course_layer);
        show_course_config_provider(show_course_window);
        window_stack_remove(swing_animation_window,false);
    */}
    //swing_animation_animator();
}

void swing_animation_window_unload(Window* window){
    layer_destroy(swing_animation_layer);
    
    checkAccel = true;
}

void shake_watch_window_load(Window* window){
    shake_watch_layer = text_layer_create(GRect(0,45,144,168-16));
    text_layer_set_background_color(shake_watch_layer,GColorClear);
    text_layer_set_text_color(shake_watch_layer, GColorBlack);
    text_layer_set_text(shake_watch_layer, "Shake\nMe!");
    text_layer_set_font(shake_watch_layer,fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(shake_watch_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window),text_layer_get_layer(shake_watch_layer));
    uint32_t num_samples = 2;
    accel_data_service_subscribe(num_samples,(AccelDataHandler)shake_watch_accel_data_handler);
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

void shake_watch_window_unload(Window* window){
    text_layer_destroy(shake_watch_layer);
}

void aim_shot_window_load(Window* window){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Aim Shot Window Loading");
    aim_shot_course_layer = layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    layer_set_update_proc(aim_shot_course_layer,aim_shot_course_layer_update_callback);
    layer_add_child(window_get_root_layer(aim_shot_window), aim_shot_course_layer);
    turnActionBar = action_bar_layer_create();
    action_bar_layer_set_background_color(turnActionBar, GColorWhite);
    counterClockwiseBitmap = gbitmap_create_with_resource(RESOURCE_ID_COUNTER_CLOCKWISE);
    clockwiseBitmap = gbitmap_create_with_resource(RESOURCE_ID_CLOCKWISE);
    checkMarkBitmap = gbitmap_create_with_resource(RESOURCE_ID_CHECK_MARK);
    action_bar_layer_set_icon(turnActionBar, BUTTON_ID_UP, counterClockwiseBitmap);
    action_bar_layer_set_icon(turnActionBar, BUTTON_ID_SELECT , checkMarkBitmap);
    action_bar_layer_set_icon(turnActionBar, BUTTON_ID_DOWN, clockwiseBitmap);
    action_bar_layer_add_to_window(turnActionBar, window);
    action_bar_layer_set_click_config_provider(turnActionBar,aim_shot_click_config_provider);
    initializeAngle();
    aiming_path = gpath_create(&AIMING_PATH_INFO);
    putter_aim = gpath_create(&PUTTER_PATH_INFO);
    iron_three_aim = gpath_create(&THREE_PATH_INFO);
    iron_five_aim = gpath_create(&FIVE_PATH_INFO);
    iron_seven_aim = gpath_create(&SEVEN_PATH_INFO);
    iron_nine_aim = gpath_create(&NINE_PATH_INFO);
    if(club_choice == PUTTER){
      aiming_path = putter_aim;
    }else if(club_choice == IRON_NINE){
      aiming_path = iron_nine_aim;
    }else if(club_choice == IRON_SEVEN){
      aiming_path = iron_seven_aim;
    }else if(club_choice == IRON_FIVE){
      aiming_path = iron_five_aim;
    }else if(club_choice == IRON_THREE){
      aiming_path = iron_three_aim;
    }
    gpath_move_to(aiming_path, GPoint(currentX,currentY));
    gpath_rotate_to(aiming_path, TRIG_MAX_ANGLE / 360 * globalAngle);
}

void aim_shot_window_unload(Window* window){
    layer_destroy(aim_shot_course_layer);
}

void pick_club_window_load(Window* window){
    pick_club_layer = menu_layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    menu_layer_set_click_config_onto_window(pick_club_layer,pick_club_window);
    MenuLayerCallbacks callbacks = {
        .draw_row     = (MenuLayerDrawRowCallback)pc_draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)pc_num_rows_callback,
        .select_click = (MenuLayerSelectCallback)pc_select_click_callback
    };
    menu_layer_set_callbacks(pick_club_layer,NULL,callbacks);
    layer_add_child(window_get_root_layer(pick_club_window),menu_layer_get_layer(pick_club_layer));
}

void pick_club_window_unload(Window* window){
    menu_layer_destroy(pick_club_layer);
}

void show_course_window_load(Window* window){
    show_course_layer = layer_create(GRect(0,0,144,168-MENU_CELL_BASIC_HEADER_HEIGHT));
    layer_set_update_proc(show_course_layer,show_course_layer_update_callback);
    layer_add_child(window_get_root_layer(show_course_window),show_course_layer);
    window_set_click_config_provider(show_course_window, (ClickConfigProvider)show_course_config_provider);
}

void show_course_window_unload(Window* window){
    gpath_destroy(hole_one);
    gpath_destroy(green_path);
    gpath_destroy(hole_path);
    gpath_destroy(sand1_path);
    gpath_destroy(sand2_path);
    gpath_destroy(sand3_path);
    gpath_destroy(teebox_path);
    layer_destroy(show_course_layer);
}

void show_course_window_appear(Window* window){
    window_set_click_config_provider(show_course_window, (ClickConfigProvider)show_course_config_provider);
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
        .load   = mm_window_load,
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
