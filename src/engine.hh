/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#pragma once

#include <psp2/rtc.h>
#include <psp2/ctrl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <soloud.h>

#include "screen.hh"

class Engine {
public:
    Engine();
    void Run();
    void SetScreen(Screen* new_screen);

    bool is_running = true;
    bool is_screen_changeable = true;
    
    int which_enter;
    int which_exit;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* surface = NULL;

    SoLoud::Soloud soloud;

    Screen* current_screen;
    
    SceCtrlData pad;
    int btnstat[12] = { 0, };
    const int btnorder[12] = { SCE_CTRL_SELECT, SCE_CTRL_START, SCE_CTRL_UP, SCE_CTRL_RIGHT,
                               SCE_CTRL_DOWN, SCE_CTRL_LEFT, SCE_CTRL_LTRIGGER, SCE_CTRL_RTRIGGER,
                               SCE_CTRL_TRIANGLE, SCE_CTRL_CIRCLE, SCE_CTRL_CROSS, SCE_CTRL_SQUARE };
};