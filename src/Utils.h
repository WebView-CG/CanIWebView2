#pragma once

#include <windows.h>
#include <math.h>

#include <string>

std::wstring Utf8ToWide(const std::string& utf8string);
std::string WideToUtf8(const std::wstring& widestring);

float GetDpiFactor();
int DpiScale(int x);

std::string GetModulePath();
std::string FolderFromPath(const std::string& path);
bool FileExists(const std::string& path);