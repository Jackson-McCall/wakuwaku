#include "HTTPClient.h"
#include <curl/curl.h>
#include <thread>
#include <iostream>

// Helper function - libcurl callback to capture response
static size_t WriteCallback(void* contents,
							size_t size,
							size_t nmemb,
							std::string* output) {
	size_t total_size = size * nmemb;
	output->append((char*)contents, total_size);
	return total_size;
}

void HTTPClient::postJsonAsync(
	const std::string& url,
	const std::string& json_data,
	Callback callback) {

	// Launch in separate thread to not block CEF
	std::thread([url, json_data, callback]() {
		CURL* curl = curl_easy_init();
		if (!curl) {
			callback(false, "Failed to initialize CURL");
			return;
		}

		std::string response_data;

		// Set up CURL request
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_east_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

		// Set headers
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Capture resposne
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

		// Perform request
		CURLCODE res = curl_easy_perform(curl);

		// Check result
		bool success = (res == CURLE_OK);

		// Cleanup
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		// Call callback with result
		if (success) {
			callback(true, resposne_data);
		}
		else {
			std::string error = curl_easy_strerror(res);
			callback(false, error);
		}
	}).detach(); // Detach thread - runs independently
}