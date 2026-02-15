#pragma once

#include "include/cef_client.h"
#include "include/cef_life_span_handler.h"
#include "include/cef_load_handler.h"
#include "include/cef_display_handler.h"
#include "handlers/LifeSpanHandler.h"
#include "handlers/LoadHandler.h"

// ============================================================================
// MinimalClient - Browser-Level CEF Handler
// ============================================================================
//
// PURPOSE:
// CefClient is the main interface for receiving callbacks from browser 
// instances. It acts as a factory pattern - CEF calls Get*Handler() methods
// to retrieve specialized handlers for different event types.
//
// LIFECYCLE:
// - Created before CreateBrowserSync()
// - One client per browser instance (but can be shared)
// - Lives as long as the browser exists
// - Destroyed after browser closes
//
// RESPONSIBILITIES:
// - Provide handlers for browser events (load, display, keyboard, etc.)
// - Route events to appropriate handler implementations
// - Can implement handler interfaces directly or return separate objects
//
// HANDLER TYPES AVAILABLE:
// - CefLifeSpanHandler: Browser creation/destruction
// - CefLoadHandler: Page loading events
// - CefDisplayHandler: UI updates (title, favicon, status)
// - CefRequestHandler: Network requests
// - CefContextMenuHandler: Right-click menus
// - CefDialogHandler: File dialogs
// - CefDownloadHandler: File downloads
// - CefDragHandler: Drag & drop
// - CefFindHandler: Find in page
// - CefFocusHandler: Focus events
// - CefKeyboardHandler: Keyboard input
// - CefJSDialogHandler: JavaScript dialogs (alert, confirm)
// - CefRenderHandler: Off-screen rendering
// - CefResourceRequestHandler: Per-request handling
//
// WHEN TO EXTEND:
// - Page load events: Override GetLoadHandler()
// - Javascript execution: Use CefFrame::ExecuteJavaScript()
// - Network interception: Override GetRequestHandler()
// - Off-screen rendering: Override GetRenderHandler()
//
// DESIGN PATTERNS:
// 1. Return nullptr - "I don't care about these events"
// 2. Return this - "I implement the handler interface myself"
// 3. Return new Handler() - "I have a separate handler object"
//
// ============================================================================

class MinimalClient : public CefClient {
public:
	MinimalClient();
	virtual ~MinimalClient() = default;

	// ------------------------------------------------------------------------
	// Handler Factory Methods
	// ------------------------------------------------------------------------
	
	// Return the LifeSpanHandler for this browser
	// LifeSpanHandler receives callbacks when browsers are created/destroyed
	//
	// EVENTS WE'D HANDLE:
	// - OnBeforePopup: Before popup window opens
	// - OnAfterCreated: After browser is created
	// - OnBeforeClose: Before browser closes
	//
	// RETURNS:
	// - nullptr: We don't care about lifetime events (current behavior)
	// - this: If MinimalClient also inherits CefLifeSpanHandler
	// - new LifeSpanHandler(): A separate handler object
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;

	// Return the LoadHandler for this browser
	// LoadHandler receives callbacks about page loading process
	// 
	// EVENTS WE'D HANDLE
	// - OnLoadStart: Page starts loading
	// - OnLoadEnd: Page finishes loading (including javascript execution!)
	// - OnLoadError: Page fails to load
	// 
	// THIS IS WHERE WE WILL SCRAPE - in OnLoadEnd after JS finishes
	// 
	// RETURNS: nullptr for now (we'll add this later)
	CefRefPtr<CefLoadHandler> GetLoadHandler() override;

	// Return the DisplayHandler for this browser
	// DisplayHandler receives callbacks about UI updates
	// 
	// EVENTS WE'D HANDLE
	// - OnTitleChange: Page title changes
	// - OnAddressChange: URL in address bar changes
	// - OnStatusMessage: Status message changes (hover over link)
	// - OnConsoleMessage: Javascript console.log() output
	// 
	// USEFUL FOR DEBUGGING - see console.log from scraped pages
	//
	// RETURNS: nullptr for now
	CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;

	// ------------------------------------------------------------------------
	// Reference Counting
	// ------------------------------------------------------------------------

	IMPLEMENT_REFCOUNTING(MinimalClient);

	// ------------------------------------------------------------------------
	// Non-copyable
	// ------------------------------------------------------------------------

	MinimalClient(const MinimalClient&) = delete;
	MinimalClient& operator=(const MinimalClient&) = delete;

private:
	CefRefPtr<LifeSpanHandler> life_span_handler_;
	CefRefPtr<LoadHandler> load_handler_;
};