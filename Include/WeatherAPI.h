#pragma once

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplxtasks.h>

#include <optional>
#include <mutex>

#include "WeatherData.h"
#include "Config.h"

class WeatherAPI
{
public:
	WeatherAPI(const utility::string_t& apiKey);

	[[nodiscard]] Concurrency::task<std::optional<WeatherData>> FetchCurrentWeather(const utility::string_t& city);
	[[nodiscard]] Concurrency::task<std::optional<WeatherForecast>> FetchForecast(const utility::string_t& city);

	const utility::string_t& GetErrorMessage() const { return errorMessage; }

private:
	const utility::string_t apiKey;

	web::http::client::http_client client;
	utility::string_t errorMessage;
	std::mutex errorMessageMutex;

private:
	Concurrency::task<web::json::value> HandleResponse(const web::http::http_response& response);

	static WeatherData GetDataFromJson(web::json::value& jsonResponse);
};