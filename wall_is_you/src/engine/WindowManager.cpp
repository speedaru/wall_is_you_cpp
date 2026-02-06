#include "WindowManager.hpp"


void WindowManager::Push(std::unique_ptr<IView> view) {
	// We defer pushing to avoid modifying the stack mid-loop
	m_pendingPush.push_back(std::move(view));
}

void WindowManager::ProcessChanges() {
	if (m_shouldPop && !m_viewStack.empty()) {
		m_viewStack.pop_back();
		m_shouldPop = false;
	}
	for (auto& view : m_pendingPush) {
		m_viewStack.push_back(std::move(view));
	}
	m_pendingPush.clear();
}

void WindowManager::HandleEvents(const sf::Event& event) {
	if (!m_viewStack.empty()) {
		m_viewStack.back()->HandleEvent(event);
	}
}

void WindowManager::Update(float dt) {
	if (!m_viewStack.empty()) {
		m_viewStack.back()->Update(dt);
	}
}

void WindowManager::Render(sf::RenderWindow& window) {
	// Find the lowest modal window or start from bottom
	size_t startIdx = 0;
	for (size_t i = m_viewStack.size(); i > 0; --i) {
		if (m_viewStack[i - 1]->IsModal() && !m_viewStack[i - 1]->IsTransparent()) {
			startIdx = i - 1;
			break;
		}
	}

	for (size_t i = startIdx; i < m_viewStack.size(); ++i) {
		m_viewStack[i]->Render(window);
	}
}

