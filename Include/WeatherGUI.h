#pragma once

#include "WeatherAPI.h"
#include "AsyncData.h"

class WeatherGUI
{
public:
    WeatherGUI(WeatherAPI& api);

    void Render();

private:
    WeatherAPI& weatherAPI;
    std::string city;

    AsyncData<WeatherData> weatherInfo;
    AsyncData<WeatherForecast> forecastInfo;

private:
    void RenderCurrentCity();
    void RenderCurrentWeather();
    void RenderForecast();
    void RenderErrorMessage() const;

    static void RenderWeatherData(const WeatherData& data);
    static void RenderWeatherDataDetails(const WeatherData& data);
    static WeatherForecast GroupForecastByDay(const WeatherForecast& forecastData);
};