#include <stdio.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <cmath>

#include "core.h"

void core::mandel_draw_path(){
    char output[256];
    int screeny = SCREEN_H;
    int screenx = SCREEN_W;

    double minx, maxx, miny, maxy;
    int i;
    int iy, ix;
    int old_iy, old_ix;
    int bailout;
    int cycle;
    int last;

    int ci, ci2;

    double cx, cy, zx, zy;
    double zxn, zyn;
    double er;

    _points *points;

    minx = -2.5;
    maxx =  1.5;
    miny = -2.0;
    maxy = miny * -1;

    er = 2.0;
    bailout = 750;

    ix = mouse_x;
    iy = mouse_y;

    cy = miny + iy* ( maxy - miny ) / screeny;
    cx = minx + ix* ( maxx - minx ) / screenx;

    zx = 0;
    zy = 0;

    old_ix = -1;
    old_iy = -1;

    cycle = bailout;
    last  = bailout;

    points = (_points*) malloc ( sizeof (_points) * bailout );

    sprintf(output, "mandel: %2.4f %2.4f\n", cx, cy);
    al_draw_text(font, al_map_rgb(255,255,255), (20), (60), ALLEGRO_ALIGN_LEFT, output);

    ci  = 0;
    ci2 = 0;

    for ( i = 0 ; i<bailout ; i ++ ) {
        zxn = zx*zx - zy*zy + cx;
        zyn = 2.0*zx*zy + cy;
        zx = zxn;
        zy = zyn;

        ix = screenx * ( zx - minx ) / (maxx - minx);
        iy = screeny * ( zy - miny ) / (maxy - miny);

        points[i].x = ix;
        points[i].y = iy;

        if ( i > 0 && draw_line ) {
            al_draw_line(ix, iy, old_ix, old_iy, al_map_rgb(0, 127, 255), 1);
        }

        if ( draw_points ) {
            al_draw_pixel(ix, iy, al_map_rgb(255, 0, 0));
        }

        old_iy = iy;
        old_ix = ix;

        if ( zx*zx + zy*zy>er*er ) {
            sprintf(output, "inside: NO");
            al_draw_text(font, al_map_rgb(255,255,255), (20), (80), ALLEGRO_ALIGN_LEFT, output);
            last = i;
            goto end;
        }
    }

    sprintf(output, "inside: YES");
    al_draw_text(font, al_map_rgb(255,255,255), (20), (80), ALLEGRO_ALIGN_LEFT, output);

    for ( i = 0 ; i < ((int) last / 2)-1 ; i ++ ) {
        ci ++  ;
        ci2+= 2;

        if ( points[ci].x - points[ci2].x == 0) {
            if ( points[ci].y - points[ci2].y == 0) {
                if ( ci2 - ci < cycle ){
                    cycle = ci2 - ci;
                }
            }
        }
    }

    sprintf(output, "Cycle: YES \t size: %d", cycle);
    al_draw_text(font, al_map_rgb(255,255,255), (20), (100), ALLEGRO_ALIGN_LEFT, output);

end:
    free(points);

    return;
}

void core::mandel_init(){
    int screeny = SCREEN_H;
    int screenx = SCREEN_W;

    double minx, maxx, miny, maxy;
    int iy, ix, i;
    int bailout;

    minx = -2.5;
    maxx =  1.5;
    miny = -2.0;
    maxy = miny * -1;

    double cx, cy, zx, zy;
    double zxn, zyn;
    double er;

    er = 2.0;
    bailout = 250;

    for ( iy = 0 ; iy<screeny ; iy ++  ) {

        cy = miny + iy* ( maxy - miny ) / screeny; // This gets the imaginary part of the complex number associated with a pixel

        for ( ix = 0 ; ix<screenx ; ix ++  ) {

            cx = minx + ix* ( maxx - minx ) / screenx; // Same as above but with the real part

            zx = 0;
            zy = 0;

            for ( i = 0 ; i<bailout ; i ++  ) {     // The main loop
                zxn = zx*zx - zy*zy + cx;     // Complex number black magic
                zyn = 2.0*zx*zy + cy;       //
                zx = zxn;
                zy = zyn;

                if ( zx*zx + zy*zy>er*er ) {  // Checks if the thing goes outside the escape radius
                    //al_put_pixel(ix, iy, al_map_rgb(255, 255, 255));
                    al_put_pixel(ix, iy, al_map_rgb(0, 0, 0));
                    break;
                }
            }

            if ( i >= bailout - 1 ) {           // If it doesnt escape the mark is as black ( zero ) 
                //al_put_pixel(ix, iy, al_map_rgb(0, 0, 0));
                al_put_pixel(ix, iy, al_map_rgb(255, 255, 255));
            }
        }

        if ( iy% ( screeny/10 )  == 0 ) {   // Shows the progress
            fprintf ( stderr,"  --  %.2f%%\n", ( iy/ ( double ) screeny ) *100.0 ) ;
        }
    }

    fprintf ( stderr,"  --  %.2f%%\n", ( iy/ ( double ) screeny ) *100.0 ) ;
}

void core::mandel_draw(){
    al_draw_bitmap(mandel, 0, 0, 0);
}

void core::loop(){
    char output[256];
    bool run;

    run = true;

    while( run ) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_get_mouse_state(&state);

        if ( state.buttons & 1 ) {

        }

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit(0);
            break;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    run = false;
                    exit(0);
                    break;

                case ALLEGRO_KEY_W:
                    key[KEY_UP] = true;
                    break;

                case ALLEGRO_KEY_S:
                    key[KEY_DOWN] = true;
                    break;

                case ALLEGRO_KEY_A:
                    key[KEY_LEFT] = true;
                    break;

                case ALLEGRO_KEY_D:
                    key[KEY_RIGHT] = true;
                    break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    key[KEY_UP] = false;
                    break;

                case ALLEGRO_KEY_S:
                    key[KEY_DOWN] = false;
                    break;

                case ALLEGRO_KEY_A:
                    key[KEY_LEFT] = false;
                    break;

                case ALLEGRO_KEY_D:
                    key[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_1:
                    draw_line = !draw_line;
                    break;

                case ALLEGRO_KEY_2:
                    draw_points = !draw_points;
                    break;

                case ALLEGRO_KEY_3:
                    draw_cursor = !draw_cursor;
                    if ( draw_cursor ) {
                        al_show_mouse_cursor(this->display);
                    } else {
                        al_hide_mouse_cursor(this->display);
                    }
                    break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            mouse_x = ev.mouse.x;
            mouse_y = ev.mouse.y;
        }

        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            //break;
        }

        int dx = 0;
        int dy = 0;

        if(key[KEY_UP]){
            mouse_y -= 1;
        }

        if(key[KEY_DOWN]){
            mouse_y += 1;
        }

        if(key[KEY_LEFT]){
            mouse_x -= 1;
        }

        if(key[KEY_RIGHT]){
            mouse_x += 1;
        }

        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            mandel_draw();
            mandel_draw_path();

            al_draw_text(font, al_map_rgb(255,255,255), (20), (20), ALLEGRO_ALIGN_LEFT, "GET REKT");

            sprintf(output, "mouse: %3d %3d\n", mouse_x, mouse_y);
            al_draw_text(font, al_map_rgb(255,255,255), (20), (40), ALLEGRO_ALIGN_LEFT, output);

            draw_fps();

            al_flip_display();

            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
}

core::core(){
    this->display       = NULL;
    this->event_queue   = NULL;
    this->timer         = NULL;
    this->font          = NULL;
    this->redraw        = true;

    for ( int i = 0 ; i < 4 ; i++ ) {
        key[i] = false;
    }

    al_init_font_addon();
    al_init_ttf_addon();

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
    }

    if(!al_install_mouse()) {
        fprintf(stderr, "failed to initialize the mouse!\n");
    }

    this->timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
    }

    this->display = al_create_display(SCREEN_W, SCREEN_H);
    if(!this->display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(this->timer);
    }

    this->font = al_load_ttf_font("pirulen.ttf",12,0 );

    if (!font){
        fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
    }

    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize al_init_image_addon!\n");
    }


    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
    }

    if(!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize the primitives engine!\n");
    }

    mandel = al_create_bitmap(SCREEN_W, SCREEN_H);

    if(mandel == NULL) {
        fprintf(stderr, "failed to initialize the mandelbrot bitmap!\n");
    }

    al_lock_bitmap(mandel, al_get_bitmap_format(mandel), ALLEGRO_LOCK_WRITEONLY);
    al_set_target_bitmap(mandel);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    mandel_init();
    al_unlock_bitmap(mandel);

    al_set_target_bitmap(al_get_backbuffer(this->display));

    this->event_queue = al_create_event_queue();
    if(!this->event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(this->display);
        al_destroy_timer(this->timer);
    }

    this->fps = 0;
    this->frames_done = 0;
    this->old_time = al_get_time();

    al_grab_mouse(this->display);
    al_hide_mouse_cursor(this->display);

    al_register_event_source(this->event_queue, al_get_display_event_source(this->display));
    al_register_event_source(this->event_queue, al_get_timer_event_source(this->timer));
    al_register_event_source(this->event_queue, al_get_mouse_event_source());
    al_register_event_source(this->event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    al_start_timer(this->timer);

    al_set_mouse_xy(display, SCREEN_W/2, SCREEN_H/2);
    this->mouse_x = SCREEN_W/2;
    this->mouse_y = SCREEN_H/2;

    draw_line   = true;
    draw_points = true;
    draw_cursor = true;
}

core::~core(){
    al_destroy_timer(this->timer);
    al_destroy_display(this->display);
    al_destroy_event_queue(this->event_queue);
}

void core::draw_fps(){
    char output[256];

    double game_time = al_get_time();
    if(game_time - old_time >= 1.0) {
        fps = frames_done / (game_time - old_time);
        frames_done = 0;
        old_time = game_time;
    }

    sprintf(output, "FPS:%0.1f\n", fps);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), (SCREEN_W-10), (20), ALLEGRO_ALIGN_RIGHT, output);

    this->frames_done++;
}
