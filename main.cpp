// ============================================================================
// main.cpp - Entry Point for WakuWaku scraper
// ============================================================================
//
// This file contains ONLY the main() function and CEF initialization
// All class implementations are in separate files
//
// ARCHITECTURE:
// - MinimalApp: Application-level CEF configuration (src/MinimalApp.h)
// - MinimalClient: Browser-level event handling (src/MinimalClient.h)
// - main(): Initializes CEF and creates browser window
//
// ============================================================================

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used Windows headers
#define NOMINMAX             // Prevent min/max macros
#include <windows.h>
#endif

// Includes
#include "include/cef_app.h"      // CefApp interface
#include "include/cef_browser.h"  // Browser creation functions
#include "src/MinimalApp.h"
#include "src/MinimalClient.h"


// ============================================================================
// Entry Point - Platform-Specific
// ============================================================================
//
// Windows GUI applications have a special entry point: APIENTRY...
// While linux/console applications use standard C main.
// Preprocessor executes the ifdef and chooses the header for main()
/*
#ifdef _WIN32
int APIENTRY wWinMain(
	HINSTANCE hInstance,  // Handle to current program instance
	HINSTANCE,            // Always NULL (legacy from Windows 3.1)
	LPWSTR,               // Command line as wide string
	int)                  // How to show window (minimize, maximize, etc.)
{
	int argc = __argc;
	char** argv = __argv;
#else
*/
int main(int argc, char* argv[]) {
#ifdef _WIN32
	CefMainArgs main_args(GetModuleHandle(nullptr)); // Wrap command-line arguments
#else
	CefMainArgs main_args(argc, argv);
#endif

	// Create application handler
	CefRefPtr<CefApp> app(new MinimalApp());

	// Handle multi-process architecture:
	// CEF spawns helper processes (renderer, GPU, etc.)
	// They run this same executable with --type= flags
	// CefExecuteProcess handles them and returns early
	int exit_code = CefExecuteProcess(main_args, app, nullptr);
	if (exit_code >= 0) {
		return exit_code; // We're a helper process, exit now
	}

	// Configure CEF settings
	CefSettings settings;
	settings.no_sandbox = 1;


	// Initialize CEF
	if (!CefInitialize(main_args, settings, app.get(), nullptr)) {
		return 1;
	}

	// Configure browser window
	// CefWindowInfo - Describes how the browser window should be created
	// Window style (popup, child, windowless), position/size, parent, title
	CefWindowInfo window_info;
	#ifdef _WIN32
	window_info.SetAsPopup(nullptr, "WakuWaku Scraper Browser");
	#else
	window_info.SetAsPopup(0, "WakuWaku Scraper Browser");
	#endif

	// Other options

	// Visible popup window
	// window_info.SetAsPopup(nullptr, "Title");

	// Windowless/headless (no visible window)
	// window_info.SetAsWindowless(nullptr);

	// Child window embedded in another app
	//window_info.SetAsChild(parent_hwnd, rect)


	// Configure browser settings
	CefBrowserSettings browser_settings;

	// Create client handler
	CefRefPtr<CefClient> client(new MinimalClient());

	std::string url = "https://seekingalpha.com/news/4551932-wall-street-observes-limited-movement-after-the-latest-cpi-report";
	// Create browser instance
	// Using CreateBrowser (async) because otherwise it tries to create a
	// browser before the helper processes are ready
	bool browser_created = CefBrowserHost::CreateBrowser(
		window_info,
		client.get(),
		url,
		browser_settings,
		nullptr,
		nullptr
	);

	if (!browser_created) {
		MessageBoxA(NULL, "Failed to initiate browser creation!", "Error", MB_OK);
		return 1;
	}
	
	 
	// Run message loop (blocks until window closes)
	CefRunMessageLoop();

	// Cleanup
	CefShutdown();

	return 0;
}