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
int main(int argc, char* argv[]) {
#endif

	// ========================================================================
	// Step 1: Wrap command-line arguments
	// ========================================================================

	#ifdef _WIN32
	CefMainArgs main_args(hInstance);
	#else
	CefMainArgs main_args(argc, argv);
	#endif

	// ========================================================================
	// Step 2: Create application handler
	// ========================================================================

	CefRefPtr<CefApp> app(new MinimalApp());

	// ========================================================================
	// Step 3: Handle multi-process architecture
	// ========================================================================

	// CEF spawns helper processes (renderer, GPU, etc.)
	// They run this same executable with --type= flags
	// CefExecuteProcess handles them and returns early
	int exit_code = CefExecuteProcess(main_args, app, nullptr);

	// Debug: Logging what happens when we execute the process
	FILE* f = fopen("execute_process_result.txt", "w");
	if (f) {
		fprintf(f, "CefExecuteProcess returned: %d\n", exit_code);
		fclose(f);
	}
	if (exit_code >= 0) {
		return exit_code; // We're a helper process, exit now
	}

	// We're the main browser process, continue initialization

	// ========================================================================
	// Step 4: Configure CEF settings
	// ========================================================================

	CefSettings settings;
	settings.size = sizeof(CefSettings);
	settings.no_sandbox = 1;

	// Only set subprocess path
	CefString(&settings.browser_subprocess_path).FromASCII("C:\\Users\\jacks\\source\\repos\\wakuwaku\\out\\build\\x64-release\\WakuWaku.exe");

	settings.log_severity = LOGSEVERITY_VERBOSE;

	// ========================================================================
// Step 5: Initialize CEF
// ========================================================================

	bool init_success = false;
	__try {
		init_success = CefInitialize(main_args, settings, app.get(), nullptr);

		FILE* success_file = fopen("C:\\Users\\jacks\\OneDrive\\Desktop\\init_success.txt", "w");
		if (success_file) {
			fprintf(success_file, "CefInitialize returned: %s\n", init_success ? "TRUE" : "FALSE");
			fclose(success_file);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		FILE* crash_file = fopen("C:\\Users\\jacks\\OneDrive\\Desktop\\init_crash.txt", "w");
		if (crash_file) {
			fprintf(crash_file, "CefInitialize CRASHED! Exception code: %lx\n", GetExceptionCode());
			fclose(crash_file);
		}
		return 1;
	}

	if (!init_success) {
		return 1;
	}

	// ========================================================================
	// Step 6: Configure browser window
	// ========================================================================

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

	// ========================================================================
	// Step 7: Configure browser settings
	// ========================================================================

	CefBrowserSettings browser_settings;
	browser_settings.size = sizeof(CefBrowserSettings);

	// ========================================================================
	// Step 8: Create client handler
	// ========================================================================

	CefRefPtr<CefClient> client(new MinimalClient());

	// ========================================================================
	// Step 9: Create browser instance
	// ========================================================================

	CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
		window_info,
		client.get(),
		"https://google.com",
		browser_settings,
		nullptr,
		nullptr
	);

	// ========================================================================
	// Step 10: Run message loop (blocks until window closes)
	// ========================================================================

	CefRunMessageLoop();

	// ========================================================================
	// Step 11: Cleanup
	// ========================================================================

	CefShutdown();

	return 0;
}