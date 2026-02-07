#include "pch.h"
#include "game/constants.hpp"
#include "utils/logging.hpp"

#include "engine/datatypes/UICommands.hpp"
#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/ThreadSafeQueue.hpp"
#include "engine/AssetManager.hpp"

#include "game/assets.hpp"
#include "game/Game.hpp"


int main(int argc, char** argv) {
	WindowManager winManager;
    UICommandQueue uiQueue;
    LogicCommandQueue logicQueue;
    AssetManager assetManager;

    ServiceLocator::Init(&winManager, &uiQueue, &logicQueue, &assetManager);

    // load assets
    assets::LoadAssets(assetManager);

    Game game(WINDOW_SIZE, "Wall Is You");
    while (game.IsRunning()) {
        game.DoFrame();
        game.RenderFrame();
    }
}

