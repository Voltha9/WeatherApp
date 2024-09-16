#include "WeatherAPI.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace pplx;
using namespace utility;

WeatherAPI::WeatherAPI(const string_t& apiKey) :
	apiKey(apiKey), client(API_URL_BASE)
{
}

task<std::optional<WeatherData>> WeatherAPI::FetchCurrentWeather(const string_t& city)
{
	return client.request(methods::GET, API_URL_WEATHER + uri::encode_data_string(city) + API_URL_APPID + apiKey + API_URL_METRICS)
		.then([this](http_response response) -> task<json::value>
		{
			return HandleResponse(response);
		})
		.then([=](json::value jsonResponse) -> std::optional<WeatherData>
		{
			if (!jsonResponse.is_null())
			{
				return GetDataFromJson(jsonResponse);
			}

			return std::nullopt;
		});
}

task<std::optional<WeatherForecast>> WeatherAPI::FetchForecast(const string_t& city)
{
	return client.request(methods::GET, API_URL_FORECAST + uri::encode_data_string(city) + API_URL_APPID + apiKey + API_URL_METRICS)
		.then([this](http_response response) -> task<json::value>
		{
			return HandleResponse(response);
		})
		.then([=](json::value jsonResponse) -> std::optional<WeatherForecast>
		{
			if (!jsonResponse.is_null())
			{
				WeatherForecast forecast;
				auto list = jsonResponse[U("list")].as_array();

				for (auto& item : list)
				{
					forecast.push_back(GetDataFromJson(item));
				}

				return forecast;
			}

			return std::nullopt;
		});
}

task<json::value> WeatherAPI::HandleResponse(const http_response& response)
{
	if (response.status_code() == status_codes::OK)
	{
		{
			std::lock_guard<std::mutex> guard(errorMessageMutex);
			errorMessage.clear();
		}

		return response.extract_json();
	}
	else
	{
		{
			std::lock_guard<std::mutex> guard(errorMessageMutex);
			errorMessage = U("[Error] Code : ") + std::to_wstring(response.status_code()) + U(" ") + response.reason_phrase() + U('\n');
		}

		return task_from_result(json::value());
	}
}

WeatherData WeatherAPI::GetDataFromJson(json::value& jsonResponse)
{
	WeatherData data;

	data.timestamp = jsonResponse[U("dt")].as_integer();

	data.temperature = (float)jsonResponse[U("main")][U("temp")].as_double();
	data.feelsLike = (float)jsonResponse[U("main")][U("feels_like")].as_double();
	data.tempMin = (float)jsonResponse[U("main")][U("temp_min")].as_double();
	data.tempMax = (float)jsonResponse[U("main")][U("temp_max")].as_double();
	data.humidity = (float)jsonResponse[U("main")][U("humidity")].as_double();
	data.pressure = (float)jsonResponse[U("main")][U("pressure")].as_double();

	if (jsonResponse.has_field(U("wind")))
	{
		data.windSpeed = (float)jsonResponse[U("wind")][U("speed")].as_double();
		if (jsonResponse[U("wind")].has_field(U("deg")))
			data.windDeg = (float)jsonResponse[U("wind")][U("deg")].as_double();
	}

	if (jsonResponse.has_field(U("clouds")))
		data.cloudCoverage = jsonResponse[U("clouds")][U("all")].as_integer();

	if (jsonResponse.has_field(U("visibility")))
		data.visibility = jsonResponse[U("visibility")].as_integer();

	if (jsonResponse.has_field(U("rain")) && jsonResponse[U("rain")].has_field(U("3h")))
		data.rainVolume = (float)jsonResponse[U("rain")][U("3h")].as_double();

	if (jsonResponse.has_field(U("snow")) && jsonResponse[U("snow")].has_field(U("3h")))
		data.snowVolume = (float)jsonResponse[U("snow")][U("3h")].as_double();

	if (jsonResponse.has_field(U("sys")))
	{
		if (jsonResponse[U("sys")].has_field(U("sunrise")))
			data.sunrise = jsonResponse[U("sys")][U("sunrise")].as_integer();
		if (jsonResponse[U("sys")].has_field(U("sunset")))
			data.sunset = jsonResponse[U("sys")][U("sunset")].as_integer();
	}

	data.description = jsonResponse[U("weather")][0][U("description")].as_string();

	return data;
}