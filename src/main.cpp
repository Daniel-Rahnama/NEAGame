#include "main.hpp"

#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

#include <windows.h>
#include <string>

int main(int argc, char *argv[]) {
    try {
        Renderer renderer(width, height);
        Controller controller;
        Game(width, height).Run(target_frame_duration, renderer, controller);
    } catch (const char* exception)  {
        MessageBoxA(NULL, exception, "Exception", MB_ICONERROR | MB_OK);
    }
    return 0;
}