
#include "Utils.h"

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