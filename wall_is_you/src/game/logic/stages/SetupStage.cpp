#include "pch.h"
#include "SetupStage.hpp"

#include "utils/logging.hpp"


void SetupStage::OnEnter() {
	LOG_D("entered setup stage\n");
}

bool SetupStage::OnUpdate(float dt) {
	return false;
}

void SetupStage::OnExit() {

}

bool SetupStage::IsFinished() const {
	return true; // runs once
}
