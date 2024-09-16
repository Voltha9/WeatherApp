#pragma once

#include <cpprest/asyncrt_utils.h>

inline const int WINDOW_WIDTH = 600;
inline const int WINDOW_HEIGHT = 800;

inline const wchar_t* API_URL_BASE = U("http://api.openweathermap.org/data/2.5/");
inline const wchar_t* API_URL_WEATHER = U("weather?q=");
inline const wchar_t* API_URL_FORECAST = U("forecast?q=");
inline const wchar_t* API_URL_METRICS = U("&units=metric");
inline const wchar_t* API_URL_APPID = U("&appid=");

inline const char* API_KEY_ENV_VAR = "WEATHER_API_KEY";

utility::string_t GetApiKey();

std::string GetUnixTimeString(time_t timestamp);

std::string GetDayOfWeek(time_t timestamp);