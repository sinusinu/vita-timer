/*
 * Copyright © 2019 sinu <cpu344@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the LICENSE file for more details.
 */

#include <psp2/kernel/processmgr.h>

#include "main.hh"

int main(int argc, char *argv[]) {
    Engine engine;
    
    MainScreen* scr = new MainScreen();
    scr->Init(&engine);
    engine.SetScreen(scr);

    engine.Run();

	sceKernelExitProcess(0);
    return 0;
}