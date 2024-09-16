#pragma once

#include <vector>
#include <string>

struct WeatherData
{
    std::wstring description;   // Weather conditions description
    int timestamp = 0;          // Unix timestamp
    float temperature = 0;      // Current temperature
    float feelsLike = 0;        // Feels like temperature
    float tempMin = 0;          // Minimum temperature
    float tempMax = 0;          // Maximum temperature
    float windSpeed = 0;        // Wind speed
    float windDeg = 0;          // Wind direction in degrees (0 = North)
    float humidity = 0;         // Humidity in percent
    float pressure = 0;         // Atmospheric pressure in hPa
    int cloudCoverage = 0;      // Cloud coverage in percent
    int visibility = 0;         // Visibility in meters
    float rainVolume = 0;       // Rain volume in mm (optional)
    float snowVolume = 0;       // Snow volume in mm (optional)
    long sunrise = 0;           // Sunrise time (UNIX timestamp)
    long sunset = 0;            // Sunset time (UNIX timestamp)
};

using WeatherForecast = std::vector<WeatherData>;