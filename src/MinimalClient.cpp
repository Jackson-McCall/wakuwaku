#include "MinimalClient.h"

// ============================================================================
// Handler Factory Method Implementations
// ============================================================================
//
// These methods are called by CF when it needs a specific handler
// Returning nullptr means "I don't care about these events"
//
// DESIGN DECISION:
// We're returning nullptr for all handlers initially because we just want to 
// see a browser window. Later, we'll add handlers as we need them:
//
// ROADMAP:
// 1. Get window showing (current state - all nullptr)
// 2. Add LoadHandler to detect when page finishes loading
// 3. Add JavaScript execution to scrape data
// 4. Add RequestHandler to intercept/modify network requests
//
// ============================================================================

MinimalClient::MinimalClient() {
	// HEAP vs. STACK
	// LifeSpanHandler handler; - Dies when out of scope
	// LifeSpanHandler* handler = new LifeSpanHandler() - We must delete this
	// CefRefPtr<LifeSpanHandler> handler(new LifeSpanHandler()) - CefRefPtr deletes automatically
	life_span_handler_ = new LifeSpanHandler();
}

CefRefPtr<CefLifeSpanHandler> MinimalClient::GetLifeSpanHandler() {
	// LifeSpanHandler handles browser lifecycle events
	//
	// WHEN WE'D USE THIS:
	// - Prevent popup windows: Override OnBeforePopup() return true
	// - Track browser creation: Override OnAfterCreated()
	// - Cleanup on close: Override OnBeforeClose()
	//
	// FOR SCRAPING:
	// Not critical initially. Useful if we want to:
	// - Block ads that open in popups
	// - Track when browsers are created/destroyed
	// - Perform cleanup when browser closes

	return life_span_handler_;
}

CefRefPtr<CefLoadHandler> MinimalClient::GetLoadHandler() {
	// LoadHandler handles page loading events
	//
	// THIS IS CRITICAL FOR SCRAPING
	// 
	// OnLoadEnd() is called when the page FULLY loads, including:
	// - HTML downloaded
	// - CSS applied
	// - Javascript executed
	// - DOM fully built
	//
	// This is where we'll execute your scraping JavaScript
	//
	// NEXT STEPS:
	// 1. Create a LoadHandler class that inherits CefLoadHandler
	// 2. Override OnLoadEnd()
	// 3. Return the LoadHandler here
	// 4. In OnLoadEnd, execute JavaScript to scrape

	return nullptr; // TODO: Return LoadHandler once we create it
}

CefRefPtr<CefDisplayHandler> MinimalClient::GetDisplayHandler() {
	// DisplayHandler handles UI updates
	//
	// USEFUL FOR DEBUGGING
	//
	// OnConsoleMessage() receives JavaScript console.log() output
	// Very helpful when debugging scraped pages
	//
	// EXAMPLE:
	// Page runs: console.log("Data loaded");
	// OnConsoleMessage receives that message
	// You can print it to see what the page is doing
	
	return nullptr; // TODO: Add this for debugging console.log
}