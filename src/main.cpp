#include "main.hpp"

#include <windows.h>
#include <memory>
#include <string>

#include "appdata.hpp"
#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

int SDL_main(int argc, char* argv[]) {
    std::string playerUsername;
    std::string playerSpritesheet;
    std::string mobSpritesheet;
    std::string playerStatsAddress;
    std::string mobStatsAddress;
    std::string difficultyLevel;
    std::string localLeaderboard;
    std::string gameMission;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-username") == 0) {
            playerUsername = argv[i+1];
        }
        if (strcmp(argv[i], "-playerSpritesheet") == 0) {
            playerSpritesheet = argv[i+1];
        }
        if (strcmp(argv[i], "-mobSpritesheet") == 0) {
            mobSpritesheet = argv[i+1];
        }
        if (strcmp(argv[i], "-playerStats") == 0) {
            playerStatsAddress = argv[i+1];
        }
        if (strcmp(argv[i], "-mobStats") == 0) {
            mobStatsAddress = argv[i+1];
        }
        if (strcmp(argv[i], "-difficultyLevel") == 0) {
            difficultyLevel = argv[i+1];
        }
        if (strcmp(argv[i], "-gameMission") == 0) {
            gameMission = argv[i+1];
        }
    }

    try {
        const std::unique_ptr<AppData> appdata = std::make_unique<AppData>(playerUsername, playerSpritesheet, mobSpritesheet, playerStatsAddress, mobStatsAddress, difficultyLevel, gameMission);
        const std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(*appdata);
        const std::unique_ptr<Controller> controller = std::make_unique<Controller>();
        const std::unique_ptr<Mixer> mixer = std::make_unique<Mixer>(*appdata);
        Game(*appdata, *renderer, *controller, *mixer).Run();
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
