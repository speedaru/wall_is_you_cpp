#pragma once
#include <pch.h>

class IView {
public:
    virtual ~IView() = default;

    // Standard hooks for the Main Thread
    virtual void HandleEvent(const sf::Event& event) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;

    // Logic hooks
    virtual bool IsModal() const { return false; } // If true, blocks input to layers below
    virtual bool IsTransparent() const { return false; } // If true, draws layers below
};