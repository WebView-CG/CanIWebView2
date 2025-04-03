#pragma once

#include <string>

struct ConfigJson {
	std::string startUrl;
	bool preferUnstableChannel;
};

ConfigJson LoadConfigJson(const std::string& path);