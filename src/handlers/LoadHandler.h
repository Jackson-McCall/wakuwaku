#pragma once

#include "include/cef_load_handler.h"

class LoadHandler : public CefLoadHandler {
public:
	LoadHandler() = default;

	// Called when page starts loading
	void OnLoadStart(CefRefPtr<CefBrowser> browser,
					 CefRefPtr<CefFrame> frame,
					 CefLoadHandler::TransitionType transition_type) override;

	// Called when page finishes loading (INCLUDING JavaScript execution)
	void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			       CefRefPtr<CefFrame> frame,
			       int httpStatusCode) override;

	// Called if page fails to load
	void OnLoadError(CefRefPtr<CefBrowser> browser,
					 CefRefPtr<CefFrame> frame,
					 CefLoadHandler::ErrorCode errorCode,
					 const CefString& errorText,
					 const CefString& failedUrl) override;

	IMPLEMENT_REFCOUNTING(LoadHandler);

};