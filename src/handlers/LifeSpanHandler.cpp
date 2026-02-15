#include "LifeSpanHandler.h"
#include "include/cef_app.h"

void LifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	// Quit the message loop when the last browser window closes
	CefQuitMessageLoop();
}

