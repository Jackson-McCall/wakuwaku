#include "TextVisitor.h"
#include "include/cef_app.h"
#include "../ScrapedData.h"
#include "../HTTPClient.h"
#include <iostream>

void TextVisitor::Visit(const CefString& string) {
	std::string json_str = string.ToString();

	std::cout << "\n=== SCRAPED DATA ===\n" << json_str << "\n====================\n" << std::endl;

	// Parse the JSON
	ScrapedData data = ScrapedData::fromJsonString(json_str);

	std::cout << "Sending to API..." << std::endl;

	// Send to API asynchronously
	HTTPClient::postJsonAsync(
		"http://127.0.0.1:8080/scrapedData",
		data.toJson(),
		[](bool success, const std::string& response) {
			if (success) {
				std::cout << "\n API Response: " << response << std::endl;
			}
			else {
				std::cerr << "\n API Error: " << response << std::endl;
			}
			CefQuitMessageLoop();
		}
	);
}