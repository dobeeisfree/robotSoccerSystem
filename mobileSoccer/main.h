#pragma once
#include "setup.h"
#include <math.h>
#include <iostream>

setup ver1;

namespace mobileCalculate {
	double a, b;
	double a2, b2;
	double Cx, Cy;
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
int mode;//����� ���� ��� ����
int xyMode_x, xyMode_y, xyMode_angle;//xy����� position, angle ����
int whichPlace, whichAngle; // ����� �Է��� ���� ����

using namespace mobileCalculate;
double theta = CV_PI / 4;

char *buffer;
CSerialComm serialComm; //SerialComm ��ü ����
						
//�κ�����ü �����
typedef struct robot {
	int _angle;
	int _x;
	int _y;
} Robot;

Robot r1; // �κ�
bool bluetooth = false; // ������� ���� ����
bool destinated = false; // �����ߴ����� ����
bool ans = false; // �信 ��� �������� ����

typedef struct vel {
	// default constructor
	vel() : vr(0), vl(0),
			isRobotAngleCall(false),
			isPositionCall(false),
			isPositionAngleCall(false),
			isRobotOutOfBound(false) { }
	int vr; // ������ ����
	int vl; // ���� ����
	bool isRobotAngleCall; // RobotAngle �Լ� �ҷ������� ����
	bool isPositionCall; // Position �Լ� �ҷ������� ����
	bool isRobotOutOfBound; // ����� ���ΰ��� ����
	bool isPositionAngleCall = false; // Position_Angle ȣ�� ����
} Vel;

Vel realtimeCheck; // �ǽð� ����ó���� ����

// ����� ������
int width = 1024;
int height = 1278; //temporary value

// ���� ������
double realX;
double realY;

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
	void rtnRobotsDirection(double x, double y);
	int angle(float x1, float y1, float x2, float y2);
	void whereisrobot(int x, int y);
	void Velocity(Robot *robot, int vl, int vr);
	void RobotAngle(Robot *robot, int desired_angle);
	void Position(Robot *robot, double x, double y);
	void Position_Angle(Robot *robot, double x, double y, int desired_angle);
	void calculatingSize(double x, double y); // ����� ������ ��ȯ
	void nSideToPosition(int whichPlace); // ������ �Լ� ȣ��
	void drawText(Mat& img_input);
	void drawLine(Mat& img_input, double x1, double y1, double x2, double y2);
	void xyMode();
	void NPlaceMode();
};