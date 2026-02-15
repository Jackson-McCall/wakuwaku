#pragma once

#include "include/cef_app.h"

// ============================================================================
// MinimalApp - Applictaion-Level CEF handler
// ============================================================================
//
// PURPOSE: 
// CefApp is CEF's main application interface. CEF calls methods on this during
// initialization, shutdown, and process setup. Every CEF application must
// provide a CefApp implementation.
//
// LIFECYCLE:
// - Created in main() before CefInitialize()
// - Lives for the entire duration of the application
// - Destroyed after CefShutdown()
//
// RESPONSIBILITIES:
// - Configure command-line switches
// - Provide process-specific behavior (browser vs. renderer)
// - Register custom schemes/protocols
// - Set up resource handling
//
// WHEN TO EXTEND:
// - Add command-line flags: Override OnBeforeCommandLineProcessing()
// - Custom renderer behavior: Override GetRenderProcessHandler()
// - Custom browser behavior: Override GetBrowserProcessHandler()
//
// ============================================================================

class MinimalApp : public CefApp {
public:
	MinimalApp() = default;
	virtual ~MinimalApp() = default;

	// ------------------------------------------------------------------------
	// Command-Line Processing
	// ------------------------------------------------------------------------

	// Called before CEF processes command-line arguments
	// Use this to add switches that modify CEF's behavior
	//
	// PARAMETERS:
	// - process_type: Empty for browser, "renderer" for renderer, etc.
	// - comamnd_line: Mutable command-line object
	//
	// EXAMPLE SWITCHES:
	// - "disable-gpu" - No GPU acceleration
	// - "disable-software-rasterizer" - No software fallback
	// - "disable-web-security" - Disable CORS (dangerous, dev only)
	// - "autoplay-policy=no-user-gesture-required" - Auto-play media
	void OnBeforeCommandLineProcessing( // PascalCase because overriding
		const CefString& processType,
		CefRefPtr<CefCommandLine> command_line) override;

	// ------------------------------------------------------------------------
	// Reference Counting
	// ------------------------------------------------------------------------

	// IMPLEMENT_REFCOUNTING generates AddRef(), Release(), HasOneRef()
	// These are required by CefBaseRefCounted (which CefApp inherits from)
	// 
	// WHY NEEDED:
	// CEF uses manual reference counting for cross-DLL compatibility
	// CefRefPtr (CEF's smart pointer) calls these automatically
	//
	// GENERATED CODE:
	// void AddRef() const override { ref_count_.AddRef(); }
	// bool Release() const override {
	//     if (ref_count_.Release()) { delete this; return true; }
	//     return false;
	// }
	// bool HasOneRef const override { return ref_count_.HasOneRef(); }
	IMPLEMENT_REFCOUNTING(MinimalApp);

	// ------------------------------------------------------------------------
	// Non-copyable
	// ------------------------------------------------------------------------

	// CEF objects should never be copied! They are managed by CefRefPtr
	// Explicitly delete copy constructor and assignment operator
	MinimalApp(const MinimalApp&) = delete;
	MinimalApp& operator=(const MinimalApp&) = delete;
};