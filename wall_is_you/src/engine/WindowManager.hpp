#pragma once
#include "pch.h"
#include "datatypes/IView.hpp"

class WindowManager {
public:
    void Push(std::unique_ptr<IView> view);

    void Pop();

    void ProcessChanges();

    void HandleEvent(const sf::RenderWindow& window, const sf::Event& event);

    void Update(float dt);

    void Render(sf::RenderWindow& window);

	inline bool IsEmpty() const { return m_viewStack.empty(); }

private:
    std::vector<std::unique_ptr<IView>> m_viewStack;
    std::vector<std::unique_ptr<IView>> m_pendingPush;
    int m_popCount{ 0 };
};