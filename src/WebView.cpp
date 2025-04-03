
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

// Global variables
static WCHAR szWindowClass[] = L"CanIWebView2";
static WCHAR szTitle[] = L"CanIWebView2";
HINSTANCE hInst = nullptr;

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
float GetDpiFactor();
int DpiScale(int x);

// WebView2 pointers
static wil::com_ptr<ICoreWebView2Controller> webviewController;
static wil::com_ptr<ICoreWebView2> webview;


int CALLBACK WinMain(
	_In_	 HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_	 LPSTR     lpCmdLine,
	_In_	 int       nCmdShow
)
{
	/////////////////////////////////////////////////////
	// Register window class
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", szTitle, NULL);
		return 1;
	}

	hInst = hInstance;

	/////////////////////////////////////////////////////
	// Create window
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		DpiScale(1200), DpiScale(900),
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", szTitle, NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/////////////////////////////////////////////////////
	// Create and initialize WebView2
	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

				// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
				env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
						if (controller != nullptr) {
							webviewController = controller;
							webviewController->get_CoreWebView2(&webview);
						}

						// Specify WebView2 settings
						wil::com_ptr<ICoreWebView2Settings> settings;
						webview->get_Settings(&settings);
						settings->put_IsScriptEnabled(TRUE);
						settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						settings->put_IsWebMessageEnabled(TRUE);

						// Resize WebView to fit the bounds of the parent window
						RECT bounds;
						GetClientRect(hWnd, &bounds);
						webviewController->put_Bounds(bounds);

						// Schedule an async task to navigate to the starting URL
						webview->Navigate(L"https://github.com/WebView-CG");

						return S_OK;
					}).Get());
				return S_OK;
			}).Get());
	

	/////////////////////////////////////////////////////
	// Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/////////////////////////////////////////////////////
// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (webviewController != nullptr) {
			RECT bounds;
			GetClientRect(hWnd, &bounds);
			webviewController->put_Bounds(bounds);
		};
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
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