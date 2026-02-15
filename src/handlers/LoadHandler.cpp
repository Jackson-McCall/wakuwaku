#include "LoadHandler.h"
#include "include/cef_app.h"
#include <iostream>

// Called when page starts loading
void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
				 CefRefPtr<CefFrame> frame,
				 CefLoadHandler::TransitionType transition_type) {
	// Only care about main frame (not iframes)
	if (frame->IsMain()) {
		std::cout << "Page started loading: " << frame->GetURL().ToString() << std::endl;
	}
}

// Called when page finishes loading (INCLUDING JavaScript execution)
void LoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
			   CefRefPtr<CefFrame> frame,
			   int httpStatusCode) {
	// Only process main frame
	if (!frame->IsMain()) {
		return;
	}

	std::cout << "Page finished loading!" << std::endl;
	std::cout << "HTTP Status: " << httpStatusCode << std::endl;
	std::cout << "URL: " << frame->GetURL().ToString() << std::endl;

	// TODO: Execute Javascript to scrape here

	// For now, quit after this page loads (we'll remove this later)
	CefQuitMessageLoop();

}

// Called if page fails to load
void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
				 CefRefPtr<CefFrame> frame,
				 CefLoadHandler::ErrorCode errorCode,
				 const CefString& errorText,
				 const CefString& failedUrl){
	if (!frame->IsMain()) {
		return;
	}

	std::cerr << "Failed to load: " << failedUrl.ToString() << std::endl;
	std::cerr << "Error: " << errorText.ToString() << std::endl;

	CefQuitMessageLoop();
}
