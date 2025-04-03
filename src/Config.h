#pragma once

#include <string>

struct ConfigJson {
	std::string startUrl;
};

ConfigJson LoadConfigJson(const std::string& path);