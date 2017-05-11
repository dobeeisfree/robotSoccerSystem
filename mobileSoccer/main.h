#pragma once
#include "robot.h"
#include "gui_text.h"
#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>
#include <math.h>
#include <iostream>
#define CAP_NUMBER 0	// Set WebCam Number

using namespace cv;
using namespace std;

VideoCapture cap(CAP_NUMBER);				// define WebCam
robot team, id1, id2 = robot();			// robot colors

double a, b;
double a2, b2;
double Cx, Cy;
double theta;
double Rx, Ry;
// id1, id2
double x_one, x_two;
double y_one, y_two;

ostringstream os_webfps;
//ostringstream os_cc_stats;
ostringstream os_id1;
ostringstream os_id2;
ostringstream os_yaxb;
ostringstream os_ya2b2;

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
	void calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2);
	void makeLine(double x1, double y1, double x2, double y2);
	double returnY(double x);
	void findCenterPoint(double x1, double y1, double x2, double y2);
	void makeTheta(double x1, double y1, double x2, double y2);
	void rtnRobotsDirection(double x, double y);
	void drawText(Mat& img_input);
	void drawLine(Mat& img_input, double x1, double y1, double x2, double y2);
};