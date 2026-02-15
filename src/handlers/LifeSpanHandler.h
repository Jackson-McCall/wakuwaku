#pragma once

#include "include/cef_life_span_handler.h"

// LifeSpanHandler inherits from CefLifeSpanHandler, Is-A relationship
// This means our class gets all of the parent's methods
class LifeSpanHandler : public CefLifeSpanHandler {
public:
	LifeSpanHandler() = default;

	// Called when browser is about to close
	// override: "this method exists in parent, override it."
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	IMPLEMENT_REFCOUNTING(LifeSpanHandler);
};