#include "pch.h"
#include "WindowManager.hpp"


void WindowManager::Push(std::unique_ptr<IView> view) {
	m_pendingPush.push_back(std::move(view));
}

void WindowManager::Pop() {
	m_popCount++;
}

void WindowManager::ProcessChanges() {
	assert(m_popCount <= m_viewStack.size());

	// handle pops
	if (m_popCount) {
		// dont pop stuff in pending view

		// pop stuff in view stack
		m_viewStack.erase(m_viewStack.end() - m_popCount, m_viewStack.end());
	}

	// handle pushes
	for (auto& view : m_pendingPush) {
		m_viewStack.push_back(std::move(view));
	}

	// reset pushes and pops
	m_pendingPush.clear();
	m_popCount = 0;
}

void WindowManager::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
	if (!m_viewStack.empty()) {
		m_viewStack.back()->HandleEvent(window, event);
	}
}

void WindowManager::Update(float dt) {
	if (!m_viewStack.empty()) {
		m_viewStack.back()->Update(dt);
	}
}

void WindowManager::Render(sf::RenderWindow& window) {
	// must always at least have 1 view
	assert(m_viewStack.size() > 0);

	// iterate backwards using index
	size_t startViewIdx = 0;
	for (int64_t i = static_cast<int64_t>(m_viewStack.size()) - 1; i >= 0; i--) {
		auto& view = m_viewStack[i];
		if (view->IsModal() && !view->IsTransparent()) {
			startViewIdx = i;
			break;
		}
	}

	// render everything starting above bottom modal non transparent view
	for (size_t i = startViewIdx; i < m_viewStack.size(); i++) {
		m_viewStack[i]->Render(window);
	}
}

