#include "setup.h"

setup::setup()
{
	cap.open(CAP_NUMBER);
}


setup::~setup()
{
}

void setup::SetVideoSize(int width, int height)
{
	/*
	Set Video Size using width, height
	*/
	cap.set(CAP_PROP_FRAME_WIDTH, width);
	cap.set(CAP_PROP_FRAME_HEIGHT, height);
}

void setup::GenerateTrackbar(char * msg, object * color, char * TrackbarName)
{
	/*
	for One window, Each Trackbar is generated.
	*/
	GuiText text = GuiText();
	text.textAppend(TrackbarName);

	// Hue (0 - 179)
	cvCreateTrackbar(text.low_h, msg, &color->LowH, 179);
	cvCreateTrackbar(text.high_h, msg, &color->HighH, 179);

	// Saturation (0 - 255)
	cvCreateTrackbar(text.low_s, msg, &color->LowS, 255);
	cvCreateTrackbar(text.high_s, msg, &color->HighS, 255);

	// Value (0 - 255)
	cvCreateTrackbar(text.low_v, msg, &color->LowV, 255);
	cvCreateTrackbar(text.high_v, msg, &color->HighV, 255);
}

void setup::SetUI(char * msg, object * teamColor, object * id1Color, object * id2Color, object * ballColor)
{
	/*
	msg: window name (we made)
	*color: robot consist of team, id1, id2 color
	*/
	// set window
	namedWindow(msg, CV_WINDOW_AUTOSIZE);

	// Create Trackbar using HSV variable
	GenerateTrackbar(msg, teamColor, "team");
	GenerateTrackbar(msg, id1Color, "id1");
	GenerateTrackbar(msg, id2Color, "id2");
	GenerateTrackbar(msg, ballColor, "ball");
}

void setup::SetRobotColors()
{
	// team color
	team.SetH(112, 179);
	team.SetS(157, 255);
	team.SetV(47, 255);

	// id1 color
	id1.SetH(148, 179);
	id1.SetS(77, 255);
	id1.SetV(47, 255);

	// id2 color
	id2.SetH(8, 39);
	id2.SetS(168, 255);
	id2.SetV(47, 255);

	// ball color
	ball.SetH(44, 75);
	ball.SetS(100, 255);
	ball.SetV(49, 255);
}
