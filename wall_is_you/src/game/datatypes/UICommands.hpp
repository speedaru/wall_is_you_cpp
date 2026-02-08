#pragma once
#include "pch.h"
#include "engine/datatypes/IView.hpp"

struct UICommand {
	enum Type {
		// generic stuff
		PushView,
		PopView,
		ExitGame,

		// more specific stuff
		PushDungeonView
	} type{};
	std::optional<std::unique_ptr<IView>> view{ std::nullopt };

	UICommand() = default;

	UICommand(Type type, std::unique_ptr<IView>& view) {
		this->type = type;
		this->view = std::move(view);
	}
};