#pragma once

#include <string>

struct ConfigJson {
	std::string startUrl;
	bool preferUnstableChannel;
	std::string additionalBrowserArguments;
	std::string hostingMode;
	std::string browserExecutableFolder;
};

ConfigJson LoadConfigJson(const std::string& path);