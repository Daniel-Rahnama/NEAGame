#include "main.hpp"

#include "appdata.hpp"
#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

#include <windows.h>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        AppData* appdata = new AppData;
        std::cout << appdata->Resources() << "F\n";
        Renderer renderer(appdata->Width(), appdata->Height(), appdata->Resources().c_str());
        Controller controller;
        Game(appdata->Width(), appdata->Height(), appdata->Resources().c_str()).Run(target_frame_duration, renderer, controller);
    } catch (const char* exception)  {
        MessageBoxA(NULL, exception, "Exception", MB_ICONERROR | MB_OK);
    }
    return 0;
}