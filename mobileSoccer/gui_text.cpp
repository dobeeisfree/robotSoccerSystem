#include "gui_text.h"
#include <string.h>

GuiText::GuiText()
{
	strcpy(low_h, "Low H ");
	strcpy(low_s, "Low S ");
	strcpy(low_v, "Low V ");
	strcpy(high_h, "High H ");
	strcpy(high_s, "High S ");
	strcpy(high_v, "High V ");
}

GuiText::~GuiText()
{
	low_h[0] = '\0';
	low_s[0] = '\0';
	low_v[0] = '\0';
	high_h[0] = '\0';
	high_s[0] = '\0';
	high_v[0] = '\0';
}

void GuiText::textAppend(char * trackbar_name)
{
	/*
	Each Trackbar get unique name.
	ex)  "Low H Team"
	*/
	strcat(low_h, trackbar_name);
	strcat(high_h, trackbar_name);
	strcat(low_s, trackbar_name);
	strcat(high_s, trackbar_name);
	strcat(low_v, trackbar_name);
	strcat(high_v, trackbar_name);
}