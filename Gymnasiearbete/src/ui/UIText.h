#pragma once

#include "FontData.h"

#include "UISprite.h"

class UIText : public UISprite
{
protected:
	std::string m_Text;

	Mesh m_Mesh;

	Mesh GenerateMesh();

public:
	UIText();
	UIText(const std::string& text);
	~UIText();

	void SetText(const std::string& text);
	std::string GetText() const;
};
