#include "main.hpp"

#include "appdata.hpp"
#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

#include <windows.h>
#include <iostream>
#include <memory>

int SDL_main(int argc, char* argv[]) {
    try {
        const std::unique_ptr<AppData> appdata = std::make_unique<AppData>();
        const std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(appdata);
        const std::unique_ptr<Controller> controller = std::make_unique<Controller>();
        const std::unique_ptr<Mixer> mixer = std::make_unique<Mixer>(appdata);
        Game(appdata, renderer, controller, mixer).Run();
    } catch (const char* exception) {
        MessageBoxA(NULL, exception, "Exception", MB_ICONERROR | MB_OK);
    } catch (const std::string& exception) {
        MessageBoxA(NULL, exception.c_str(), "Exception", MB_ICONERROR | MB_OK);
    } catch (const std::exception& exception) {
        MessageBoxA(NULL, exception.what(), "Exception", MB_ICONERROR | MB_OK);
    } catch (...) {
        MessageBoxA(NULL, "Unknown exception.", "Exception", MB_ICONERROR | MB_OK);
    }
    return 0;
}