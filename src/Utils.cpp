
#include "Utils.h"

// Conversion between std::string and std::wstring
std::wstring Utf8ToWide(const std::string& utf8string)
{
	if (utf8string.empty())
		return std::wstring();

	int wcharcount = MultiByteToWideChar(CP_UTF8, 0, utf8string.data(), (int)utf8string.size(), NULL, 0);

	if (wcharcount <= 0)
		return std::wstring();

	std::wstring buffer;
	buffer.resize(wcharcount);

	MultiByteToWideChar(CP_UTF8, 0, utf8string.data(), (int)utf8string.size(), &(buffer.front()), wcharcount);

	return buffer;
}

std::string WideToUtf8(const std::wstring& widestring)
{
	if (widestring.empty())
		return std::string();

	int bytecount = WideCharToMultiByte(CP_UTF8, 0, widestring.data(), (int)widestring.size(), NULL, 0, NULL, NULL);

	if (bytecount <= 0)
		return std::string();

	std::string buffer;
	buffer.resize(bytecount);

	WideCharToMultiByte(CP_UTF8, 0, widestring.data(), (int)widestring.size(), &(buffer.front()), bytecount, NULL, NULL);

	return buffer;
}

// DPI scaling methods
float GetDpiFactor()
{
	// Lazy-load DPI factor
	static float dpi_factor = 0.0f;

	if (dpi_factor == 0.0f)
	{
		HDC screendc = ::GetDC(NULL);
		int screenDpiX = ::GetDeviceCaps(screendc, LOGPIXELSX);
		::ReleaseDC(NULL, screendc);

		// Determine DPI factor as float, relative to 96 dpi
		dpi_factor = static_cast<float>(screenDpiX) / 96.0f;
	}

	return dpi_factor;
}

int DpiScale(int x)
{
	return static_cast<int>(ceilf(static_cast<float>(x) * GetDpiFactor()));
}

// Return module path as a wstring
std::string GetModulePath()
{
	wchar_t* strbuf = new wchar_t[MAX_PATH];

	GetModuleFileName(NULL, strbuf, MAX_PATH);

	std::string ret = WideToUtf8(strbuf);
	delete[] strbuf;
	return ret;
}

// Return the folder from a given path, including trailing backslash if any.
// e.g. C:\Folder\File.ext -> C:\Folder\ 
std::string FolderFromPath(const std::string& path)
{
	std::string::size_type last_backslash = path.rfind('\\');

	if (last_backslash == std::string::npos)
		return path;			// return string unmodified
	else
		return path.substr(0, last_backslash + 1);	// include backslash
}

bool FileExists(const std::string& path)
{
	std::wstring pathW = Utf8ToWide(path);
	return GetFileAttributes(pathW.c_str()) != INVALID_FILE_ATTRIBUTES;
}