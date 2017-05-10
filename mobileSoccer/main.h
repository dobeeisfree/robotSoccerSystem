#pragma once
#include "robot.h"
#include "gui_text.h"
#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>
#include <iostream>
#define CAP_NUMBER 0	// Set WebCam Number

using namespace cv;
using namespace std;

VideoCapture cap(CAP_NUMBER);				// define WebCam
robot team, id1, id2 = robot();			// robot colors

class VisionSystem
{
public:
	VisionSystem();
	~VisionSystem();
	void SetVideoSize(int width, int height);
	void GenerateTrackbar(char * msg, robot * color, char * TrackbarName);
	void SetUI(char * msg, robot * teamColor, robot * id1Color, robot * id2Color);
	void SetRobotColors();
	void drawAreaBox(Mat img_input, Mat stats, int numOfLables, char * title);
	void VisionStart();
	void calculateTheLine(double Cx, double Cy, int left, int top, int width, int height);
};
