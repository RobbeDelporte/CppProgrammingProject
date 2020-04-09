#ifndef ALLKIT_H
#define ALLKIT_H

#include <vector>
#include <string>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"

#include "Color.h"
#include "Constants.h"
#include "Point.h"
#include "Sprite.h"

class Allkit {
public:
    // No copy constructor
    Allkit(const Allkit&) = delete;
    // No copy assignment operator
    Allkit& operator=(const Allkit&) = delete;

    static Allkit& Get()
    {
        // Memory for instance is only created once!
        static Allkit instance;
        return instance;
    }

    enum Align {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    void Init();
    void Destroy();

    void StartTimer();
    void StopTimer();
    
    void LoadLaunchSound();
    void PlayLaunchSound();

    void NextEvent();

    bool IsArrowKeyUpPushed();
    bool IsArrowKeyDownPushed();
    bool IsEnterKeyPushed();
    bool IsSpaceBarPushed();
    bool IsMouseClicked();
    bool IsMouseReleased();
    bool HasMouseMoved();
    Point& GetMouse();

    bool IsScreenActive();
    bool IsTimerEvent();
    bool IsWindowClosed();

    void ClearScreen();
    void DrawOnScreen();

    void DrawLine(Point& a, Point& b);
    void DrawPoly(std::vector<Point>& body);
    void DrawString(std::string& str, Point& location, Color& color, Align align, bool huge_font);
    void DrawScaledBitmap(Sprite sprite, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);

private:
    // Private default constructor
    Allkit() = default;

    void ShowError(std::string msg);

    int GetAlign(Allkit::Align align);

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_FONT* big_font = NULL;
    ALLEGRO_SAMPLE* launch = NULL;
    std::vector<ALLEGRO_BITMAP *> sprites;

    Point mouse;
};

#endif