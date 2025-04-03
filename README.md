# CanIWebView2
This is an MIT-licensed test app for Microsoft Edge WebView2 for the WebView CG. It is written in C++ using the Win32 API for WebView2 and is built with a Visual Studio 2022 solution. Some pre-compiled binaries are provided in the releases section.

## config.json
By default the app just opens this repository's GitHub URL in WebView2. You may wish to customize the source code for something like a bug report. However it can also be used as a binary using config.json in the same folder as the executable to configure how it works. A sample minimal config.json is shown below, and the available options are documented underneath.

```json
{
	"startUrl": "https://github.com/WebView-CG/CanIWebView2"
}
```

Supported options are:

- `"startUrl"`: string of the initial URL to load in the webview
- `"preferUnstableChannel"`: a boolean which you can set to `true` to reverse the usual channel search order. The default channel search order is the WebView2 Runtime, Beta, Dev, and Canary; setting this to `true` reverses that so it will prefer unstable channels. See [Test upcoming APIs and features](https://learn.microsoft.com/en-us/microsoft-edge/webview2/how-to/set-preview-channel) for more details. This comes down to a call to: `options->put_ChannelSearchKind(COREWEBVIEW2_CHANNEL_SEARCH_KIND_LEAST_STABLE);`
- `"additionalBrowserArguments"`: a string of additional browser arguments to pass to WebView2. This comes down to a call to: `options->put_AdditionalBrowserArguments(...);`
- `"hostingMode"`: by default the webview is created in *Windowed* hosting mode. This option can be set to the string `"window-to-visual"` to instead create WebView2 in *Window to visual* hosting mode. See [Windowed vs. Visual hosting of WebView2](https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/windowed-vs-visual-hosting) for more details. This comes down to a call to: `SetEnvironmentVariable(L"COREWEBVIEW2_FORCED_HOSTING_MODE", L"COREWEBVIEW2_HOSTING_MODE_WINDOW_TO_VISUAL");`
- `"browserExecutableFolder"`: a string of a folder where a fixed distribution of WebView2 may be found. For example this could be set to `"fixeddist"` and WebView2 will look for a fixed distribution in a subfolder named "fixeddist" in the folder the app executable belongs to. If this is specified and the fixed distribution of WebView2 is either missing or fails to load, then the app will fail to start up and show an error message. When not specified, this defaults to the evergreen distribution mode. For more details see [Distribute your app and the WebView2 Runtime](https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/distribution). This option effectively controls the `browserExecutableFolder` parameter of `CreateCoreWebView2EnvironmentWithOptions()`.
- `"userDataFolder"`: a string of a folder where WebView2 should store user-specific data, including settings and data for storage APIs like IndexedDB. For example this could be set to `"userdata"` and WebView2 will store user data in a subfolder named "userdata" in the folder the app executable belongs to. When not specified, this lets WebView2 choose a default location. This option effectively controls the `userDataFolder` parameter of `CreateCoreWebView2EnvironmentWithOptions()`.