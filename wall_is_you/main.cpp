#include "pch.h"
#include "game/constants.hpp"
#include "utils/logging.hpp"

#include "engine/datatypes/UICommands.hpp"
#include "engine/WindowManager.hpp"
#include "engine/ThreadSafeQueue.hpp"
#include "engine/ServiceLocator.hpp"

#include "game/Game.hpp"


int main(int argc, char** argv) {
	WindowManager winManager;
    ThreadSafeQueue<UICommand> threadSafeQueue;

    ServiceLocator::Init(&winManager, &threadSafeQueue);

    Game game(WINDOW_SIZE, "Wall Is You");

    while (game.IsRunning()) {
        game.DoFrame();
    }
}

