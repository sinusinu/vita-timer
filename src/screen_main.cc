/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include <cstdio>
#include <cstring>

#include "engine.hh"
#include "screen.hh"
#include "buttons.hh"

void MainScreen::init(Engine* engine) {
    en = engine;
    
    memset(&tick, 0, sizeof(tick));
    sceRtcGetCurrentTick(&tick);

    // load textures
    SDL_Surface* tmp = IMG_Load("app0:/assets/back.png");
    tx_back = SDL_CreateTextureFromSurface(en->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = IMG_Load("app0:/assets/number.png");
    tx_number = SDL_CreateTextureFromSurface(en->renderer, tmp);
    SDL_FreeSurface(tmp);
    if (en->which_enter == SCE_CTRL_CIRCLE) tmp = IMG_Load("app0:/assets/hud_asia.png");
    else tmp = IMG_Load("app0:/assets/hud_us.png");
    tx_hud = SDL_CreateTextureFromSurface(en->renderer, tmp);
    SDL_FreeSurface(tmp);

    // load sounds
    sl_beep.load("app0:/assets/beep.wav");
    sl_beep.setLooping(true);
    sl_g.load("app0:/assets/g.wav");
    sl_c.load("app0:/assets/c.wav");
}

void MainScreen::update() {
    // update
    sceRtcGetCurrentTick(&tick);

    if (mode == 0 && stopwatch_status == 1) {
        // update stopwatch clock
        SceUInt64 elapsed_time = (tick.tick - stopwatch_started_time) / 1000000;
        int sec = elapsed_time % 60;
        int min = (elapsed_time / 60) % 60;
        int hrs = (elapsed_time / 3600) % 100;
        clock_disp[0] = hrs / 10;
        clock_disp[1] = hrs % 10;
        clock_disp[3] = min / 10;
        clock_disp[4] = min % 10;
        clock_disp[6] = sec / 10;
        clock_disp[7] = sec % 10;
    } else if (mode == 1 && timer_status == 1) {
        if (timer_target_time < tick.tick) {
            // time's up, switch to finished state
            timer_status = 3;
            rect_hud_draw = &rect_hud_t_finished;
            slh_beep = en->soloud.play(sl_beep);
        } else {
            // update timer clock
            SceUInt64 time_left = (timer_target_time - tick.tick) / 1000000;
            int sec = time_left % 60;
            int min = (time_left / 60) % 60;
            int hrs = (time_left / 3600) % 100;
            clock_disp[0] = hrs / 10;
            clock_disp[1] = hrs % 10;
            clock_disp[3] = min / 10;
            clock_disp[4] = min % 10;
            clock_disp[6] = sec / 10;
            clock_disp[7] = sec % 10;
        }
    } else if (mode == 1 && timer_status == 3) {
        // blink clock
        is_clock_visible = (((tick.tick - timer_target_time) / 500000) & 1 == 1);
    }

    // render
    SDL_RenderClear(en->renderer);

    SDL_RenderCopy(en->renderer, tx_back, &rect_back, &rect_back);

    if (is_clock_visible) draw_clock();

    SDL_RenderCopy(en->renderer, tx_number, rect_number_title_draw, &rect_number_title_dest);
    SDL_RenderCopy(en->renderer, tx_hud, rect_hud_draw, &rect_hud_dest);
    
    if (mode == 1 && timer_status == 0) {
        SDL_RenderCopy(en->renderer, tx_number, &rect_number_u, &rect_number_u_dest);
        SDL_RenderCopy(en->renderer, tx_number, &rect_number_d, &rect_number_d_dest);
    }

    SDL_RenderPresent(en->renderer);
}

void MainScreen::reset_clock() {
    clock_disp[0] = 0;
    clock_disp[1] = 0;
    clock_disp[3] = 0;
    clock_disp[4] = 0;
    clock_disp[6] = 0;
    clock_disp[7] = 0;
}

void MainScreen::draw_clock() {
    for (int i = 0; i < 8; i++) {
        SDL_Rect* rect_num;
        switch (clock_disp[i]) {
            case 0: rect_num = &rect_number_0; break;   // this looks so dumb
            case 1: rect_num = &rect_number_1; break;
            case 2: rect_num = &rect_number_2; break;
            case 3: rect_num = &rect_number_3; break;
            case 4: rect_num = &rect_number_4; break;
            case 5: rect_num = &rect_number_5; break;
            case 6: rect_num = &rect_number_6; break;
            case 7: rect_num = &rect_number_7; break;
            case 8: rect_num = &rect_number_8; break;
            case 9: rect_num = &rect_number_9; break;
            case 10: rect_num = &rect_number_c; break;
        }
        rect_number_dest.x = 80 + (100 * i);
        SDL_RenderCopy(en->renderer, tx_number, rect_num, &rect_number_dest);
    }
}

void MainScreen::key_down(int btn) {
    if (btn == en->which_enter) {
        if (mode == 0) {
            // stopwatch
            switch (stopwatch_status) {
                case 0:
                    // idle: start stopwatch
                    stopwatch_status = 1;
                    reset_clock();
                    stopwatch_started_time = tick.tick;
                    rect_hud_draw = &rect_hud_s_running;
                    en->soloud.play(sl_c);
                    break;
                case 1:
                    // running: pause stopwatch
                    stopwatch_status = 2;
                    stopwatch_paused_time = tick.tick;
                    rect_hud_draw = &rect_hud_s_paused;
                    en->soloud.play(sl_g);
                    break;
                case 2:
                    // paused: resume stopwatch
                    stopwatch_status = 1;
                    stopwatch_started_time += tick.tick - stopwatch_paused_time;
                    rect_hud_draw = &rect_hud_s_running;
                    en->soloud.play(sl_c);
                    break;
            }
        } else {
            // timer
            switch (timer_status) {
                case 0:
                    // idle: start timer
                    {
                        SceUInt64 ttime = (clock_disp[7])
                                        + (clock_disp[6] * 10)
                                        + (clock_disp[4] * 60)
                                        + (clock_disp[3] * 600)
                                        + (clock_disp[1] * 3600)
                                        + (clock_disp[0] * 36000);
                        if (ttime == 0) {
                            timer_status = 3;
                            rect_hud_draw = &rect_hud_t_finished;
                            slh_beep = en->soloud.play(sl_beep);
                        } else {
                            timer_status = 1;
                            ttime = ttime * 1000000;
                            timer_target_time = tick.tick + ttime;
                            timer_started_time = tick.tick;
                            rect_hud_draw = &rect_hud_t_running;
                            en->soloud.play(sl_c);
                        }
                    }
                    break;
                case 1:
                    // running: pause timer
                    timer_status = 2;
                    timer_paused_time = tick.tick;
                    rect_hud_draw = &rect_hud_t_paused;
                    en->soloud.play(sl_g);
                    break;
                case 2:
                    // paused: resume timer
                    timer_status = 1;
                    timer_target_time += (tick.tick - timer_paused_time);
                    timer_started_time += (tick.tick - timer_paused_time);
                    rect_hud_draw = &rect_hud_t_running;
                    en->soloud.play(sl_c);
                    break;
                case 3:
                    // finished: reset
                    timer_status = 0;
                    timer_editing_digit = 0;
                    rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
                    rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
                    reset_clock();
                    is_clock_visible = true;
                    rect_hud_draw = &rect_hud_t_idle;
                    en->soloud.stop(slh_beep);
                    en->soloud.play(sl_c);
                    break;
            }
        }
    } else if (btn == en->which_exit) {
        if (mode == 0) {
            // stopwatch
            switch (stopwatch_status) {
                case 1:
                    // running: reset
                    stopwatch_status = 0;
                    reset_clock();
                    rect_hud_draw = &rect_hud_s_idle;
                    en->soloud.play(sl_c);
                    break;
                case 2:
                    // paused: reset
                    stopwatch_status = 0;
                    reset_clock();
                    rect_hud_draw = &rect_hud_s_idle;
                    en->soloud.play(sl_c);
                    break;
            }
        } else {
            // timer
            switch (timer_status) {
                case 0:
                    // idle: reset
                    timer_editing_digit = 0;
                    rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
                    rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
                    reset_clock();
                    rect_hud_draw = &rect_hud_t_idle;
                    en->soloud.play(sl_c);
                    break;
                case 1:
                    // running: reset
                    timer_status = 0;
                    timer_editing_digit = 0;
                    rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
                    rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
                    reset_clock();
                    rect_hud_draw = &rect_hud_t_idle;
                    en->soloud.play(sl_c);
                    break;
                case 2:
                    // paused: reset
                    timer_status = 0;
                    timer_editing_digit = 0;
                    rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
                    rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
                    reset_clock();
                    rect_hud_draw = &rect_hud_t_idle;
                    en->soloud.play(sl_c);
                    break;
            }
        }
    } else if (btn == SCE_CTRL_TRIANGLE) {
        if ((mode == 0 && stopwatch_status == 0) || (mode == 1 && timer_status == 0)) {
            // switch modes
            mode = (mode + 1) % 2;
            switch (mode) {
                case 0: 
                    rect_number_title_draw = &rect_number_s;
                    rect_hud_draw = &rect_hud_s_idle;
                    break;
                case 1:
                    rect_number_title_draw = &rect_number_t;
                    rect_hud_draw = &rect_hud_t_idle;
                    break;
            }
            reset_clock();
            en->soloud.play(sl_c);
        }
    } else if (btn == SCE_CTRL_LEFT) {
        if (mode == 1 && timer_status == 0) {
            timer_editing_digit -= 1;
            if (timer_editing_digit < 0) timer_editing_digit = 0;
            if (timer_editing_digit == 2 || timer_editing_digit == 5) timer_editing_digit--;
            rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
            rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
            en->soloud.play(sl_g);
        }
    } else if (btn == SCE_CTRL_RIGHT) {
        if (mode == 1 && timer_status == 0) {
            timer_editing_digit += 1;
            if (timer_editing_digit > 7) timer_editing_digit = 7;
            if (timer_editing_digit == 2 || timer_editing_digit == 5) timer_editing_digit++;
            rect_number_u_dest.x = 80 + (100 * timer_editing_digit);
            rect_number_d_dest.x = 80 + (100 * timer_editing_digit);
            en->soloud.play(sl_g);
        }
    } else if (btn == SCE_CTRL_UP) {
        if (mode == 1 && timer_status == 0) {
            switch (timer_editing_digit) {
                case 1:
                case 4:
                case 7:
                case 0:
                    clock_disp[timer_editing_digit]++;
                    if (clock_disp[timer_editing_digit] == 10) {
                        clock_disp[timer_editing_digit] = 0;
                    }
                    break;
                case 3:
                case 6:
                    clock_disp[timer_editing_digit]++;
                    if (clock_disp[timer_editing_digit] == 6) {
                        clock_disp[timer_editing_digit] = 0;
                    }
                    break;
            }
            en->soloud.play(sl_g);
        }
    } else if (btn == SCE_CTRL_DOWN) {
        if (mode == 1 && timer_status == 0) {
            switch (timer_editing_digit) {
                case 1:
                case 4:
                case 7:
                case 0:
                    clock_disp[timer_editing_digit]--;
                    if (clock_disp[timer_editing_digit] == -1) {
                        clock_disp[timer_editing_digit] = 9;
                    }
                    break;
                case 3:
                case 6:
                    clock_disp[timer_editing_digit]--;
                    if (clock_disp[timer_editing_digit] == -1) {
                        clock_disp[timer_editing_digit] = 5;
                    }
                    break;
            }
            en->soloud.play(sl_g);
        }
    }
}

void MainScreen::key_up(int btn) {
    // nothing to do
}