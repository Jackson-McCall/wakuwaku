#pragma once

#include "include/cef_string_visitor.h"

class TextVisitor : public CefStringVisitor {
public:
	TextVisitor() = default;

	void Visit(const CefString& string) override;

	IMPLEMENT_REFCOUNTING(TextVisitor);
};