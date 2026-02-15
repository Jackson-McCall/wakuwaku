#pragma once

#include <string>
#include <functional>

// Async HTTP client to send data back to the API
class HTTPClient {
public:
	// Callback: (success, response_body)
	using Callback = std::function<void(bool, const std::string&)>;

	// POST JSON to API endpoint asynchronously
	static void postJsonAsync(
		const std::string& url,
		const std::string& json_data,
		Callback callback
	);
};