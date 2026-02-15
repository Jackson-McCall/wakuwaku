#include "MinimalApp.h"

// ============================================================================
// OnBeforeCommandLineProcessing Implementation
// ============================================================================
//
// This is called BEFORE CEF interprets command-line arguments
// Perfect place to inject switches that modify CEF's behavior
//
// COMMON USE CASES:
// 1. Disable features for scraping (GPU, images, Javascript)
// 2. Enable features for debugging (remote debugging port)
// 3. Modify security settings (disable web security for testing)
// 4. Performance tuning (disable animations, reduce memory)
//
// ============================================================================

void MinimalApp::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line) {

	// Currently, we don't add any switches
	// The default CEF configuration is being used

	// FUTURE ADDITIONS - Uncomment as needed:

	// Disable GPU acceleration (good for headless servers)
	// command_line->AppendSwitch("disable-gpu");

	// Disable software rasterizer fallback
	// command_line->AppendSwitch("disable-software-rasterizer");

	// Enables remote debugging on port 0222
	// Allows Chrome DevTools to connect remotely
	// command_line->AppendSwitchWithValue("remote-debugging-port", "9222");

	// Disable image (faster scraping if you don't need them)
	// command_line->AppendSwitch("blink-settings=imagesEnabled=false");

	// Autoplay media without user gesture
	// command_line->AppendSwitchWithValue("autoplay-policy",
	//                                     "no-user-gesture-required");

	// Disable web security (DANGEROUS - dev only)
	// Disables CORS, mixed content blocking, etc.
	// command_line->AppendSwitch("disable-web-security");

	// Mute audio
	// command_line->AppendSwitch("mute-audio");
}