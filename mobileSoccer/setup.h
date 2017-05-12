#pragma once
#include "gui_text.h"
#include "object.h"
#include "TextPutOnFrame.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#define CAP_NUMBER 0						// Set WebCam Number

using namespace cv;
using namespace std;

using cv::VideoCapture;
using cv::namedWindow;
using cv::CAP_PROP_FRAME_WIDTH;
using cv::CAP_PROP_FRAME_HEIGHT;


class setup
{
public:
	VideoCapture cap;							// define WebCam
	object team, id1, id2, ball = object();		// objects colors
	TextPutOnFrame putstring;					// text on WebCam

	setup();
	~setup();

	void SetVideoSize(int width, int height);
	void GenerateTrackbar(char * msg, object * color, char * TrackbarName);
	void SetUI(char * msg, object * teamColor, object * id1Color, object * id2Color, object * ballColor);
	void SetRobotColors();
};

