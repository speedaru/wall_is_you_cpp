#pragma once
#include "pch.h"
#include "datatypes/IView.hpp"

class WindowManager {
public:
    void Push(std::unique_ptr<IView> view);

	inline void Pop() { m_shouldPop = true; }

    void ProcessChanges();

    void HandleEvents(const sf::Event& event);

    void Update(float dt);

    void Render(sf::RenderWindow& window);

	inline bool IsEmpty() const { return m_viewStack.empty(); }

private:
    std::vector<std::unique_ptr<IView>> m_viewStack;
    std::vector<std::unique_ptr<IView>> m_pendingPush;
    bool m_shouldPop = false;
};