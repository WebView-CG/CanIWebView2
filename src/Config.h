#pragma once

#include <string>

struct ConfigJson {
	std::string startUrl;
	bool preferUnstableChannel;
	std::string additionalBrowserArguments;
};

ConfigJson LoadConfigJson(const std::string& path);