#include "LoadHandler.h"
#include "include/cef_app.h"
#include <iostream>
#include <thread>
#include <chrono>

// Add this class at the TOP of LoadHandler.cpp, before any functions
class TextVisitor : public CefStringVisitor {
public:
	void Visit(const CefString& string) override {
		std::cout << "\n=== SCRAPED DATA ===\n" << std::endl;
		std::cout << string.ToString() << std::endl;
		std::cout << "\n====================\n" << std::endl;
		CefQuitMessageLoop();
	}
	IMPLEMENT_REFCOUNTING(TextVisitor);
};


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

	// Execute JavaScript to scrape the page
	std::string js_code = R"(
		(function() {
			// Extract title (data-test-id attribute)
			let titleElement = document.querySelector('h1[data-test-id="post-title"]');
			let title = titleElement ? titleElement.innerText.trim() : 'Title not found';

			// Extract all paragraph text (including paywall content)
			let paragraphs = document.querySelectorAll('p');
			let articleText = '';
            paragraphs.forEach(p => {
				let text = p.innerText.trim();
				if (text.length > 0) {
					articleText += text + '\n\n';
				}
			});
			
			// Return as JSON String
			let result = JSON.stringify({
				title: title,
				body : articleText,
				url: window.location.href
			}, null, 2);

			// Replace entire page with just our JSON for now
			document.body.innerHTML = '<pre>' + result + '</pre>';
		})();
	)";

	// Execute the Javascript
	frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);

	std::cout << "JavaScript executed - extracting data..." << std::endl;
	std::cout << "Extracting article data..." << std::endl;

	// Wait a moment for the JS to run, then get results
	std::this_thread::sleep_for(std::chrono::seconds(2));

	frame->GetText(new TextVisitor());


	// For now, quit after this page loads (we'll remove this later)
	//CefQuitMessageLoop();

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
