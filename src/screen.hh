/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#pragma once

#include <psp2/rtc.h>
#include <SDL2/SDL.h>
#include <soloud_wav.h>

class Engine;

class Screen {
public:
    virtual void init(Engine* engine) = 0;
    virtual void update() = 0;
    virtual void key_down(int btn) = 0;
    virtual void key_up(int btn) = 0;
};

class MainScreen : public Screen {
public:
    void init(Engine* engine);
    void update();
    void key_down(int btn);
    void key_up(int btn);
    
    void reset_clock();
    void draw_clock();
    
    Engine* en;

    SDL_Texture* tx_back;
    SDL_Rect rect_back = { 0, 0, 960, 544 };
    SDL_Texture* tx_number;
    SDL_Rect rect_number_dest = { 0, 197, 100, 150 };
    SDL_Rect rect_number_0 = { 0, 0, 100, 150 };
    SDL_Rect rect_number_1 = { 100, 0, 100, 150 };
    SDL_Rect rect_number_2 = { 200, 0, 100, 150 };
    SDL_Rect rect_number_3 = { 300, 0, 100, 150 };
    SDL_Rect rect_number_4 = { 400, 0, 100, 150 };
    SDL_Rect rect_number_5 = { 0, 150, 100, 150 };
    SDL_Rect rect_number_6 = { 100, 150, 100, 150 };
    SDL_Rect rect_number_7 = { 200, 150, 100, 150 };
    SDL_Rect rect_number_8 = { 300, 150, 100, 150 };
    SDL_Rect rect_number_9 = { 400, 150, 100, 150 };
    SDL_Rect rect_number_c = { 0, 300, 100, 150 };  // colon
    SDL_Rect rect_number_title_dest = { 330, 60, 300, 75 };
    SDL_Rect rect_number_s = { 100, 300, 300, 75 }; // stopwatch title
    SDL_Rect rect_number_t = { 100, 375, 300, 75 }; // timer title
    SDL_Rect rect_number_u_dest = { 80, 123, 100, 75 };
    SDL_Rect rect_number_d_dest = { 80, 337, 100, 75 };
    SDL_Rect rect_number_u = { 400, 300, 100, 75 }; // up arrow
    SDL_Rect rect_number_d = { 400, 375, 100, 75 }; // down arrow
    SDL_Texture* tx_hud;
    SDL_Rect rect_hud_dest = { 224, 480, 512, 24 };
    SDL_Rect rect_hud_s_idle = {0, 0, 512, 24};
    SDL_Rect rect_hud_s_running = {0, 24, 512, 24};
    SDL_Rect rect_hud_s_paused = {0, 48, 512, 24};
    SDL_Rect rect_hud_t_idle = {0, 72, 512, 24};
    SDL_Rect rect_hud_t_running = {0, 96, 512, 24};
    SDL_Rect rect_hud_t_paused = {0, 120, 512, 24};
    SDL_Rect rect_hud_t_finished = {0, 144, 512, 24};

    SDL_Rect* rect_number_title_draw = &rect_number_s;
    SDL_Rect* rect_hud_draw = &rect_hud_s_idle;

    SoLoud::Wav sl_beep;
    SoLoud::Wav sl_g;
    SoLoud::Wav sl_c;
    int slh_beep;

    int mode = 0;
    int bottom_text = 0;
    
    SceRtcTick tick;

    // 0 = idle, 1 = running, 2 = paused
    int stopwatch_status = 0;
    SceUInt64 stopwatch_started_time = 0;
    SceUInt64 stopwatch_paused_time = 0;

    // 0 - idle, 1 = running, 2 = paused, 3 = finished
    int timer_status = 0;
    int timer_editing_digit = 0;
    SceUInt64 timer_target_time = 0;
    SceUInt64 timer_started_time = 0;
    SceUInt64 timer_paused_time = 0;

    bool is_clock_visible = true;
    int clock_disp[8] = {0, 0, 10, 0, 0, 10, 0, 0};

    // timer for preventing system from entering suspend mode
    SceUInt64 powertick_last_tick_time = 0;
    const SceUInt64 powertick_interval = 10 * 1000 * 1000;
};