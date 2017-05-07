#pragma once

class GuiText
{
public:
	char low_h[40];
	char low_s[40];
	char low_v[40];
	char high_h[40];
	char high_s[40];
	char high_v[40];

	GuiText();
	~GuiText();
	void textAppend(char * trackbar_name);
};