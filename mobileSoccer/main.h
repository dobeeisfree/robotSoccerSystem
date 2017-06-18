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

Pos n1, n2, n3, n4; // N 사분면 정의를 위한 변수
int mode;//사용자 선택 모드 변수
int xyMode_x, xyMode_y, xyMode_angle;//xy모드의 position, angle 변수
int whichPlace, whichAngle; // 사용자 입력을 위한 변수

using namespace mobileCalculate;
double theta = CV_PI / 4;

char *buffer;
CSerialComm serialComm; //SerialComm 객체 생성
						
//로봇구조체 선언부
typedef struct robot {
	int _angle;
	int _x;
	int _y;
} Robot;

Robot r1; // 로봇
bool bluetooth = false; // 블루투스 연결 여부
bool destinated = false; // 도착했는지의 여부
bool ans = false; // 답에 충실 했으면의 여부

typedef struct vel {
	// default constructor
	vel() : vr(0), vl(0),
			isRobotAngleCall(false),
			isPositionCall(false),
			isPositionAngleCall(false),
			isRobotOutOfBound(false) { }
	int vr; // 오른쪽 바퀴
	int vl; // 왼쪽 바퀴
	bool isRobotAngleCall; // RobotAngle 함수 불렀는지의 여부
	bool isPositionCall; // Position 함수 불렀는지의 여부
	bool isRobotOutOfBound; // 경기장 밖인가의 여부
	bool isPositionAngleCall = false; // Position_Angle 호출 여부
} Vel;

Vel realtimeCheck; // 실시간 비전처리를 위함

// 경기장 사이즈
int width = 1024;
int height = 1278; //temporary value

// 실제 사이즈
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
	void calculatingSize(double x, double y); // 경기장 사이즈 변환
	void nSideToPosition(int whichPlace); // 포지션 함수 호출
	void drawText(Mat& img_input);
	void drawLine(Mat& img_input, double x1, double y1, double x2, double y2);
	void xyMode();
	void NPlaceMode();
};