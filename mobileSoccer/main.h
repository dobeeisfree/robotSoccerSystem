#pragma once
#include "setup.h"
#include <math.h>
#include <iostream>

setup ver1;

namespace mobileCalculate {
	double a, b;
	double a2, b2;
	double Cx, Cy;
	double theta;
	double Rx, Ry;
	// id1, id2
	double x_id1, x_id2;
	double y_id1, y_id2;
	// ball
	double x_ball, y_ball;
}

typedef struct _Pos{
	int _x;
	int _y;
} Pos;

Pos n1, n2, n3, n4; // N ��и� ���Ǹ� ���� ����
int whichPlace, whichAngle; // ����� �Է��� ���� ����
int robot_vr, robot_vl; // �κ��� ����, ������ ���� for PWM

using namespace mobileCalculate;

char buffer;
CSerialComm serialComm; //SerialComm ��ü ����

class VisionSystem
{
public:
	VisionSystem();
	~VisionSystem();
	
	void start();
	void setFourSides();
	void centerPoint(char * title, int left, int top, int width, int height);
	void drawAreaBox(Mat img_input, Mat stats, int numOfLables, char * title);
	void calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2);
	void makeLine(double x1, double y1, double x2, double y2);
	void findCenterPoint(double x1, double y1, double x2, double y2);
	void makeTheta();
	void rtnRobotsDirection(double x, double y);
	int angle(float x1, float y1, float x2, float y2);
	void whereisrobot(int x, int y);
	void blueTooth();
	void autoPosition(int robotCx, int robotCy, int desired_x, int desired_y, int whichAngle);
	void drawText(Mat& img_input);
	void drawLine(Mat& img_input, double x1, double y1, double x2, double y2);
};