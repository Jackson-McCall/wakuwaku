#pragma once

#include <string>
#include <sstream>

struct ScrapedData {
	std::string title;
	std::string body;
	std::string url;

	// Escape special characters for JSON
	static std::string escapeJson(const std::string& str) {
		std::ostringstream o;
		for (char c : str) {
			switch (c) {
				case '"': o << "\\\""; break;
				case '\\': o << "\\\\"; break;
				case '\b': o << "\\b"; break;
				case '\f': o << "\\f"; break;
				case '\n': o << "\\n"; break;
				case '\r': o << "\\r"; break;
				case '\t': o << "\\t"; break;
				default: o << c; break;
			}
		}
		return o.str();
	}

	// Convert to JSON string
	std::string toJson() const {
		std::ostringstream json;
		json << "{\n";
		json << "  \"title\": \"" << escapeJson(title) << "\",\n";
		json << "  \"body\": \"" << escapeJson(body) << "\",\n";
		json << "  \"url\": \"" << escapeJson(url) << "\"\n";
		json << "}";
		return json.str();
	}

	// Parse from JSON string (simple parser for our format)
	static ScrapedData fromJsonString(const std::string& json_str) {
		ScrapedData data;

		// Very basic parser

		// Finds "title" string, goes to end of it, then finds next string
		// That next string is our 
		// Same with body, etc.
		size_t title_start = json_str.find("\"title\": \"") + 10;
		size_t title_end = json_str.find("\"", title_start);
		data.title = json_str.substr(title_start, title_end - title_start);
		
		size_t body_start = json_str.find("\"body\": \"") + 9;
		size_t body_end = json_str.find("\"", body_start);
		data.body = json_str.substr(body_start, body_end - body_start);

		size_t url_start = json_str.find("\"url\": \"") + 8;
		size_t url_end = json_str.find("\"", url_start);
		data.url = json_str.substr(url_start, url_end - url_start);

		return data;
	}
};

