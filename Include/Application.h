#pragma once

#include <SFML/Graphics.hpp>

#include "WeatherGUI.h"

class Application
{
public:
    Application();
    ~Application();

    bool IsRunning() const;

    void Events();
    void Update();
    void Render();

private:
    sf::RenderWindow window;
    sf::Clock deltaClock;

    WeatherAPI weatherAPI;
    WeatherGUI weatherGUI;

private:
    void SetupGuiStyle();
    void SetupWindowIcon();
};