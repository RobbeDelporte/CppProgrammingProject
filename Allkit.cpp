#include "Allkit.h"

void Allkit::Init()
{
    // Init Allegro
    // Atexit routine
    if (!al_init()) {
        ShowError("Failed to initialize allegro!");
        exit(-1);
    }

    // Init image addon
    // Shutdown is done automatically at program exit
    if (!al_init_image_addon()) {
        ShowError("Failed to initialize al_init_image_addon!");
        exit(-1);
    }

    // Init primitives addon
    // Shutdown is done automatically at program exit
    if (!al_init_primitives_addon()) {
        ShowError("Failed to initialize al_init_primitives_addon!");
        exit(-1);
    }

    // Init audio subsystem
    if(!al_install_audio()) {
        ShowError("Failed to initialize audio!");
        exit(-1);
    }

    // Registers all known audio file type handlers
    if (!al_init_acodec_addon()) {
        ShowError("Failed to initialize acodec addon!");
        exit(-1);
    }

    // Reserve 1 audio sample
    if (!al_reserve_samples(1)) {
        ShowError("Audio sample could not be reserved!");
        exit(-1);
    }

    // Init font addon
    // Shutdown is done automatically at program exit
    if (!al_init_font_addon()) {
        ShowError("Failed to initialize font addon!");
        exit(-1);
    }

    // Bring support for .ttf fonts
    if (!al_init_ttf_addon()) {
        ShowError("Failed to initialize TTF addon!");
        exit(-1);
    }

    // Install keyboard driver
    // Uninstall automatically called when Allegro is shut down
    if (!al_install_keyboard()) {
        ShowError("Failed to initialize the keyboard!");
        exit(-1);
    }

    // Install mouse driver
    // Uninstall automatically called when Allegro is shut down
    if (!al_install_mouse()) {
        ShowError("Failed to initialize the mouse!");
        exit(-1);
    }

    // Load sprite cache
    sprites.resize(SPRT_LENGTH);
    sprites[SPRT_BACKGROUND] = al_load_bitmap("./assets/images/background.png");
    sprites[SPRT_MENU_BACKGROUND] = al_load_bitmap("./assets/images/menu_background.png");
    sprites[SPRT_MISSILE_1] = al_load_bitmap("./assets/images/missile_1.png");
    sprites[SPRT_MISSILE_2] = al_load_bitmap("./assets/images/missile_2.png");
    sprites[SPRT_MISSILE_3] = al_load_bitmap("./assets/images/missile_3.png");
    sprites[SPRT_LAUNCHER] = al_load_bitmap("./assets/images/launcher.png");
    sprites[SPRT_TARGET] = al_load_bitmap("./assets/images/target.png");
    sprites[SPRT_TARGET_HIT] = al_load_bitmap("./assets/images/target_hit.png");
    sprites[SPRT_BOX] = al_load_bitmap("./assets/images/box.png");
    sprites[SPRT_BOX_HIT] = al_load_bitmap("./assets/images/box_hit.png");
    sprites[SPRT_STONE] = al_load_bitmap("./assets/images/stone.png");
    sprites[SPRT_STONE_HIT] = al_load_bitmap("./assets/images/stone_hit.png");
    sprites[SPRT_CSTONE] = al_load_bitmap("./assets/images/stone_brick.png");
    sprites[SPRT_FIRECHARGE] = al_load_bitmap("./assets/images/fire_charge.png");
    sprites[SPRT_OBSIDIAN] = al_load_bitmap("./assets/images/obsidian.png");
    sprites[SPRT_OBSIDIAN_HIT] = al_load_bitmap("./assets/images/obsidian_hit.png");
    sprites[SPRT_SLIME1] = al_load_bitmap("./assets/images/slime1.png");
    sprites[SPRT_SLIME2] = al_load_bitmap("./assets/images/slime2.png");
    sprites[SPRT_BEE1] = al_load_bitmap("./assets/images/bee1.png");
    sprites[SPRT_ROCK] = al_load_bitmap("./assets/images/rock.png");

    LoadLaunchSound();

    // Load normal font
    font = al_load_ttf_font("./assets/fonts/digital.ttf", 48, 0);
    if (!font) {
        ShowError("Failed to initialize font!");
        exit(-1);
    }

    // Load big font
    big_font = al_load_ttf_font("./assets/fonts/digital.ttf", 80, 0);
    if (!big_font) {
        ShowError("Failed to initialize big_font!");
        exit(-1);
    }

    // Create timer
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        ShowError("Failed to create the timer!");
        exit(-1);
    }

    // Create Display
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display)
    {
        ShowError("Failed to initialize display!");
        al_destroy_timer(timer);
        exit(-1);
    }
    
    // Create event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        ShowError("Failed to create an event_queue!");
        al_destroy_display(display);
        al_destroy_timer(timer);
        exit(-1);
    }

    // Register different event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
}

void Allkit::StartTimer() {
    al_start_timer(timer);
}

void Allkit::StopTimer() {
    al_stop_timer(timer);
}

void Allkit::Destroy() {
    for (std::vector<ALLEGRO_BITMAP *>::iterator iter = sprites.begin(); iter != sprites.end(); iter++)
    {
        if ((*iter) != NULL)
            al_destroy_bitmap(*iter);
    }

    if (event_queue != NULL)
    {
        al_unregister_event_source(event_queue, al_get_mouse_event_source());
        al_unregister_event_source(event_queue, al_get_keyboard_event_source());
        al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
        al_unregister_event_source(event_queue, al_get_display_event_source(display));
        al_destroy_event_queue(event_queue);
    }

    al_destroy_font(font);
    al_destroy_font(big_font);
    al_destroy_sample(launch);

    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();

    al_uninstall_mouse();
    al_uninstall_keyboard();

    al_destroy_display(display);
}

////////////////////////////////////

void Allkit::ClearScreen() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Allkit::DrawOnScreen() {
    al_flip_display();
}

////////////////////////////////////

void Allkit::DrawLine(Point& a, Point& b) {
    al_draw_line(a.x_, a.y_, b.x_, b.y_, al_map_rgb(0, 0, 0), 3);
}

void Allkit::DrawPoly(std::vector<Point>& body) {
    for (unsigned int i = 0; i < body.size(); i++) {
        al_draw_line(body[i].x_,
                     body[i].y_,
                     body[(i + 1) % body.size()].x_,
                     body[(i + 1) % body.size()].y_,
                     al_map_rgb(255, 0, 0), 3);
    }
}

void Allkit::DrawString(std::string& str, Point& location, Color& color, Align align, bool huge_font) {
    al_draw_text(huge_font ? big_font : font,
                 al_map_rgb(color.r_, color.g_, color.b_), 
                 location.x_, 
                 location.y_, 
                 GetAlign(align), 
                 str.c_str());
}

void Allkit::DrawScaledBitmap(Sprite sprite, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) {
    al_draw_scaled_bitmap(sprites[sprite], sx, sy, sw, sh, dx, dy, dw, dh, 0);
}

int Allkit::GetAlign(Allkit::Align align) {
    int a;
    switch (align) {
    case Allkit::ALIGN_LEFT:
        a = ALLEGRO_ALIGN_LEFT;
        break;
    case Allkit::ALIGN_CENTER:
        a = ALLEGRO_ALIGN_CENTRE;
        break;
    case Allkit::ALIGN_RIGHT:
        a = ALLEGRO_ALIGN_RIGHT;
        break;
    }
    return a;
}

////////////////////////////////////
////////// ERROR HANDLING //////////
////////////////////////////////////

// Display error dialog to user
void Allkit::ShowError(std::string msg) {
    al_show_native_message_box(display, "Error", "Error", msg.c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

////////////////////////////////////
////////// EVENT HANDLING //////////
////////////////////////////////////

// Fetch next event from the queue
void Allkit::NextEvent() {
    al_wait_for_event(event_queue, &event);
}

// Check whether arrow key up is pushed by user
bool Allkit::IsArrowKeyUpPushed() {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_UP;
}

// Check whether arrow key down is pushed by user
bool Allkit::IsArrowKeyDownPushed() {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_DOWN;
}

// Check whether enter key is pushed by user
bool Allkit::IsEnterKeyPushed() {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER);
}

bool Allkit::IsSpaceBarPushed() {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE;
}

// Check whether current screen is the active one
bool Allkit::IsScreenActive() {
    return event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN;
}

// Check whether timer event is raised
bool Allkit::IsTimerEvent() {
    return event.type == ALLEGRO_EVENT_TIMER;
}

// Check whether user wants to quit the application
bool Allkit::IsWindowClosed(){
    return event.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

// Check whether mouse is clicked
bool Allkit::IsMouseClicked() {
    return event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1;
}

// Check whether mouse is released
bool Allkit::IsMouseReleased() {
    return event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.button == 1;
}

bool Allkit::HasMouseMoved() {
    return event.type == ALLEGRO_EVENT_MOUSE_AXES;
}

Point& Allkit::GetMouse() {
    mouse.x_ = event.mouse.x;
    mouse.y_ = event.mouse.y;
    return mouse;
}

////////////////////////////////////
////////////// AUDIO ///////////////
////////////////////////////////////

void Allkit::LoadLaunchSound() {
    launch = al_load_sample("./assets/sounds/launch_sound.wav");
}

void Allkit::PlayLaunchSound() {
    al_play_sample(launch, 1.0, 0.3, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}