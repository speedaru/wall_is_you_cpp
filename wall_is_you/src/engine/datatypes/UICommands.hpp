#pragma once
#include "pch.h"

struct UICommand {
	enum Type {
		PushView,
		PopView,
		ExitGame
	} type;
    int viewId; 
};