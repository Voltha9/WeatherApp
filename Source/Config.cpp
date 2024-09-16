#include "Config.h"

utility::string_t GetApiKey()
{
	char* apiKey = nullptr;
	size_t len = 0;
	errno_t err = _dupenv_s(&apiKey, &len, API_KEY_ENV_VAR);

	if (err != 0 || apiKey == nullptr)
	{
		std::cerr << "The API key is not defined. Please set the " << API_KEY_ENV_VAR << " environment variable." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	utility::string_t result = utility::conversions::to_string_t(apiKey);
	free(apiKey);

	return result;
}

std::string GetUnixTimeString(time_t unixTime)
{
	tm tm;
	localtime_s(&tm, &unixTime);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");
	return oss.str();
}

std::string GetDayOfWeek(time_t timestamp)
{
	tm tm;
	localtime_s(&tm, &timestamp);

	char buffer[10];
	strftime(buffer, sizeof(buffer), "%A", &tm);
	return buffer;
}