#pragma once
#include "pch.h"
#include "IView.hpp"

struct UICommand {
	enum Type {
		PushView,
		PopView,
		ExitGame
	} type;
	std::unique_ptr<IView> view = nullptr;

	UICommand() = default;

	UICommand(Type type, std::unique_ptr<IView>& view) {
		this->type = type;
		this->view = std::move(view);
	}

	//UICommand(UICommand&& other) noexcept {
	//	type = type;
	//	view = std::move(other.view);
	//}
};