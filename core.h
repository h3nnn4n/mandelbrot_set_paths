#ifndef  __core_class
#define  __core_class

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <vector>

const float FPS = 60;
const int SCREEN_W = 1366;
const int SCREEN_H = 768;

enum MYKEYS {
       KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

typedef struct{
    int x, y;
} _points;

class core{
    private:
        ALLEGRO_DISPLAY     *display;
        ALLEGRO_BITMAP      *mandel;
        ALLEGRO_EVENT_QUEUE *event_queue;
        ALLEGRO_TIMER       *timer;
        ALLEGRO_FONT        *font;
        ALLEGRO_MOUSE_STATE state;

        bool redraw;
        bool key[4];

        bool draw_line;
        bool draw_points;
        bool draw_cursor;

        int mouse_x;
        int mouse_y;

        double fps;
        double old_time;
        int frames_done;
    public:
        core();
        ~core();

        void draw_fps();

        void loop();

        void mandel_init();
        void mandel_draw();
        void mandel_draw_path();
};


#endif // __core_class
