
#include <fstream>

#include "Config.h"
#include "Utils.h"
#include "json.hpp"

ConfigJson LoadConfigJson(const std::string& path)
{
	// Set defaults
	ConfigJson ret;
	ret.startUrl = "https://github.com/WebView-CG/CanIWebView2";
	ret.preferUnstableChannel = false;

	// If file does not exist, just use defaults
	if (!FileExists(path))
		return ret;

	// Parse JSON from file. Report an error if reading/parsing fails.
	nlohmann::json parsedJson;

	try {
		std::ifstream f(path);
		parsedJson = nlohmann::json::parse(f);
	}
	catch (...)
	{
		MessageBox(NULL, L"Failed to parse config.json. Check the file has valid JSON and that all parameters are correct.", L"CanIWebView2", MB_ICONERROR);
		return ret;
	}

	if (parsedJson.contains("startUrl"))
		ret.startUrl = parsedJson["startUrl"].get<std::string>();

	if (parsedJson.contains("preferUnstableChannel"))
		ret.preferUnstableChannel = parsedJson["preferUnstableChannel"].get<bool>();

	return ret;
}