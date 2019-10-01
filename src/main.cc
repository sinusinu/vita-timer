/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include "main.hh"

int main(int argc, char *argv[]) {
    Engine engine;
    
    MainScreen* scr = new MainScreen();
    scr->init(&engine);
    engine.setScreen(scr);

    engine.run();

    return 0;
}