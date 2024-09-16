#include "WeatherGUI.h"

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <unordered_map>
#include <algorithm>
#include <ranges>

WeatherGUI::WeatherGUI(WeatherAPI& api) : 
    weatherAPI(api), 
    city("Paris")
{
    weatherInfo.ResetTask(weatherAPI.FetchCurrentWeather(utility::conversions::to_string_t(city)));
    forecastInfo.ResetTask(weatherAPI.FetchForecast(utility::conversions::to_string_t(city)));
}

void WeatherGUI::Render()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Weather App", nullptr, ImGuiWindowFlags_NoDecoration);

    RenderCurrentCity();
    ImGui::Separator();

    RenderCurrentWeather();
    ImGui::Separator();

    RenderForecast();
    ImGui::End();
}

void WeatherGUI::RenderCurrentCity()
{
    static char city_cstr[128] = "Paris";
    ImGui::TextColored(ImVec4(0.0f, 0.6f, 0.9f, 1.0f), "Enter City:");
    if (ImGui::InputText("##City", city_cstr, IM_ARRAYSIZE(city_cstr)))
    {
        city = city_cstr;
    }
}

void WeatherGUI::RenderCurrentWeather()
{
    if (ImGui::Button("Get Current Weather"))
    {
        weatherInfo.ResetTask(weatherAPI.FetchCurrentWeather(utility::conversions::to_string_t(city)));
    }

    weatherInfo.RetrieveData();

    if (weatherInfo.data)
    {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Day: %s", GetDayOfWeek(time(nullptr)).c_str());
        RenderWeatherData(*weatherInfo.data);
        if (ImGui::CollapsingHeader("More Details"))
        {
            RenderWeatherDataDetails(*weatherInfo.data);
        }
    }
    else
    {
        RenderErrorMessage();
    }
}

void WeatherGUI::RenderForecast()
{
    if (ImGui::Button("Get Forecast"))
    {
        forecastInfo.ResetTask(weatherAPI.FetchForecast(utility::conversions::to_string_t(city)));
    }

    forecastInfo.RetrieveData();

    if (forecastInfo.data)
    {
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.5f, 1.0f), "5-Day Forecast");

        if (ImGui::BeginTable("ForecastTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX))
        {
            auto dailyForecasts = GroupForecastByDay(*forecastInfo.data);

            for (auto& forecast : dailyForecasts)
            {
                ImGui::TableNextColumn();
                ImGui::BeginGroup();
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Day: %s", GetDayOfWeek(forecast.timestamp).c_str());
                RenderWeatherData(forecast);

                std::string headerLabel = "More Details##" + std::to_string(forecast.timestamp);
                if (ImGui::CollapsingHeader(headerLabel.c_str()))
                {
                    RenderWeatherDataDetails(forecast);
                }
                ImGui::EndGroup();
            }

            ImGui::EndTable();
        }
    }
    else
    {
        RenderErrorMessage();
    }
}

void WeatherGUI::RenderErrorMessage() const
{
    ImGui::TextColored(ImVec4(1, 0, 0, 1), utility::conversions::to_utf8string(weatherAPI.GetErrorMessage()).c_str());
}

void WeatherGUI::RenderWeatherData(const WeatherData& data)
{
    ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "Temperature: %.2f C", data.temperature);
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Feels Like: %.2f C", data.feelsLike);
    ImGui::Text("Min Temperature: %.2f C", data.tempMin);
    ImGui::Text("Max Temperature: %.2f C", data.tempMax);

    ImGui::Text("Conditions: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), utility::conversions::to_utf8string(data.description).c_str());
}

void WeatherGUI::RenderWeatherDataDetails(const WeatherData& data)
{
    ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.3f, 1.0f), "Humidity: %.2f %%", data.humidity);
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 0.9f, 1.0f), "Wind Speed: %.2f m/s", data.windSpeed);
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 0.9f, 1.0f), "Wind Direction: %.2f %s", data.windDeg, u8"\u00B0");
    ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.2f, 1.0f), "Cloud Coverage: %d %%", data.cloudCoverage);
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.6f, 1.0f), "Visibility: %d m", data.visibility);

    if (data.rainVolume > 0)
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.9f, 1.0f), "Rain Volume: %.2f mm", data.rainVolume);

    if (data.snowVolume > 0)
        ImGui::TextColored(ImVec4(0.9f, 0.9f, 1.0f, 1.0f), "Snow Volume: %.2f mm", data.snowVolume);

    if (data.sunrise > 0)
        ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.3f, 1.0f), "Sunrise: %s", GetUnixTimeString(data.sunrise).c_str());

    if (data.sunset > 0)
        ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.3f, 1.0f), "Sunset: %s", GetUnixTimeString(data.sunset).c_str());
}

WeatherForecast WeatherGUI::GroupForecastByDay(const WeatherForecast& forecastData)
{
    std::unordered_map<std::string, WeatherData> dailyForecasts;

    for (const auto& data : forecastData)
    {
        time_t time = static_cast<time_t>(data.timestamp);
        std::string day = GetDayOfWeek(time);

        if (!dailyForecasts.contains(day))
        {
            dailyForecasts[day] = data;
            dailyForecasts[day].tempMin = data.tempMin;
            dailyForecasts[day].tempMax = data.tempMax;
        }
        else
        {
            auto& forecast = dailyForecasts[day];
            forecast.tempMin = std::min(forecast.tempMin, data.tempMin);
            forecast.tempMax = std::max(forecast.tempMax, data.tempMax);
            forecast.description = data.description;
        }
    }

    dailyForecasts.erase(GetDayOfWeek(time(nullptr)));

    WeatherForecast sortedForecasts;

    for (const auto& value : std::views::values(dailyForecasts))
    {
        sortedForecasts.push_back(value);
    }

    std::ranges::sort(sortedForecasts, {}, &WeatherData::timestamp);

    return sortedForecasts;
}