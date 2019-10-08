/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include <cstdio>
#include <cstring>
#include <psp2/apputil.h>
#include <psp2/system_param.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <soloud.h>

#include "engine.hh"

Engine::Engine() {
    // get which button is enter
    int which = 0;
    sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &which);
    which_enter = (which == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE) ? SCE_CTRL_CIRCLE : SCE_CTRL_CROSS;
    which_exit  = (which == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE) ? SCE_CTRL_CROSS : SCE_CTRL_CIRCLE;
    
    memset(&pad, 0, sizeof(pad));

    // init sdl and things
    SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
    
    window = SDL_CreateWindow("vita-timer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    surface = SDL_GetWindowSurface(window);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_GL_SetSwapInterval(1);

    // init soloud
    soloud.init();
}

void Engine::run() {
    // main loop
    while (is_running == true) {
        is_screen_changeable = false;
        sceCtrlReadBufferPositive(0, &pad, 1);
        for (int i = 0; i < 12; i++) {  // there could be a better way to do this...
            if ((pad.buttons & btnorder[i]) != btnstat[i]) {
                if ((pad.buttons & btnorder[i]) > 0) current_screen->key_down(btnorder[i]);
                else current_screen->key_up(btnorder[i]);
                btnstat[i] = pad.buttons & btnorder[i];
            }
        }
        is_screen_changeable = true;

        current_screen->update();
    }
    
    // cleanup
    // is this necessary? seems like this won't be called anyway
    soloud.deinit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
	IMG_Quit();
    SDL_Quit();
}

void Engine::setScreen(Screen* new_screen) {
    if (is_screen_changeable == false) throw;   // do not change screen on key event function...?
                                                // this protection is meaningless for this app since it uses only one screen anyway
                                                // this part was copypasted from another project, and I'm lazy enough to leave it as is...
    current_screen = new_screen;
}