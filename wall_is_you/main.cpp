#include "pch.h"
#include "game/constants.hpp"
#include "utils/logging.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/ThreadSafeQueue.hpp"
#include "engine/AssetManager.hpp"

#include "game/datatypes/UICommands.hpp"
#include "game/datatypes/LogicCommands.hpp"
#include "game/assets.hpp"
#include "game/Game.hpp"


int main(int argc, char** argv) {
	sp::WindowManager winManager;
    sp::ThreadSafeQueue<UICommand> uiQueue;
    sp::ThreadSafeQueue<LogicCommand> logicQueue;
    sp::AssetManager assetManager;

    sp::ServiceLocator::Init(&winManager, &uiQueue, &logicQueue, &assetManager);

    // load assets
    assets::LoadAssets(assetManager);

    Game game(WINDOW_SIZE, "Wall Is You");
    while (game.IsRunning()) {
        game.DoFrame();
        game.RenderFrame();
    }
}

