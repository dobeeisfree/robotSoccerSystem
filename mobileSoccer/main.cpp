#include "main.h"

VisionSystem::VisionSystem()
{
	ver1.SetVideoSize(640, 480);
	ver1.SetRobotColors();
	ver1.SetUI("[Setup] Team, ID1, ID2, Ball", &ver1.team, &ver1.id1, &ver1.id2, &ver1.ball);
	cout << "VisionSystem version 2017.05.15 7:15PM \n";
}

VisionSystem::~VisionSystem()
{
}

void VisionSystem::centerPoint(char * title, int left, int top, int width, int height)
{
	if (strcmp(title, "id1") == 0)
	{
		x_id1 = round((double)(left + (double)(width / 2)));
		y_id1 = round((double)(top + (double)(height / 2)));
	}
	else if (strcmp(title, "id2") == 0)
	{
		x_id2 = round((double)(left + (double)(width / 2)));
		y_id2 = round((double)(top + (double)(height / 2)));
	}
	else if (strcmp(title, "ball") == 0)
	{
		x_ball = round((double)(left + (double)(width / 2)));
		y_ball = round((double)(top + (double)(height / 2)));
	}
	else return;
}

void VisionSystem::drawAreaBox(Mat img_input, Mat stats, int numOfLables, char * title)
{
	int max = -1;
	int idx = 0;
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);
		if (max < area)
		{
			max = area;
			idx = j;
		}
	}

	int left = stats.at<int>(idx, CC_STAT_LEFT);
	int top = stats.at<int>(idx, CC_STAT_TOP);
	int width = stats.at<int>(idx, CC_STAT_WIDTH);
	int height = stats.at<int>(idx, CC_STAT_HEIGHT);
	
	// Center point
	centerPoint(title, left, top, width, height);

	// draw reactangle
	if (strcmp(title, "ball") == 0)
	{
		rectangle(img_input, Point(left, top), Point(left + width, top + height), Scalar(0, 255, 255), 1);
	}
	else
	{
		rectangle(img_input, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 1);
	}
}

void VisionSystem::drawLine(Mat& img_input, double x1, double y1, double x2, double y2)
{
	line(img_input, cvPoint(Rx, Ry), cvPoint(Cx, Cy), CV_RGB(0, 0, 0), 3, 8, 0);	   // 방향선
	line(img_input, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(255, 255, 255), 1, 8, 0); // 중심선
}

void VisionSystem::makeLine(double x1, double y1, double x2, double y2)
{
	// 직선의 방정식 기본 형태 :  y = ax + b
	// 두점이 (x1, y1), (x2, y2)라고 할때 기울기 a는 다음과 같다.
	// a = (y2 - y1) / (x2 - x1)
	a = double(y2 - y1) / (x2 - x1);

	// y = ax + b 이고 ax를 넘기면 아래와 같은 공식이 성립한다.
	// b = y - ax;
	b = y2 - (a*x2);
}

void VisionSystem::findCenterPoint(double x1, double y1, double x2, double y2)
{
	Cx = (x1 + x2) / 2;
	Cy = (y1 + y2) / 2; //find robot's center point
}

void VisionSystem::makeTheta()
{
	//theta = atan((y1 - y2) / (x1 - x2)) - (3.1415f / 4);
	theta = (CV_PI / 4);
	//theta = 20;
}

void VisionSystem::rtnRobotsDirection(double x, double y)
{
	//회전식: 
	x = x - Cx;
	y = y - Cy;
	Rx = (x)*cos(theta) - (y)*sin(theta);
	Ry = (y)*cos(theta) + (x)*sin(theta);

	Rx += Cx;
	Ry += Cy;

	//좌표의 형태 
	ver1.putstring.os_yaxb.name << "connected line within two idcolors: y = " << a << "x + " << b;

	a2 = double(Ry - Cy) / (Rx - Cx);
	b2 = Ry - a2*Rx;
	ver1.putstring.os_ya2b2.name << "direction line: " << "y = " << a2 << "x + " << b2;
}


void VisionSystem::calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2)
{
	makeLine(x1, y1, x2, y2);
	findCenterPoint(x1, y1, x2, y2);
	makeTheta();
	rtnRobotsDirection(x1, y1);
	drawLine(img_input, x1, y1, x2, y2);
}

void VisionSystem::drawText(Mat& img_input)
{
	ver1.putstring.os_id1.name << "id1 (" << x_id1 << ", " << y_id1 << ")";
	ver1.putstring.os_id2.name << "id2 (" << x_id2 << ", " << y_id2 << ")";
	ver1.putstring.os_ball.name << "ball (" << x_ball << ", " << y_ball << ")";

	// start (10, 320)
	ver1.putstring.setTextPosition(10, 320);
	ver1.putstring.onFrame(img_input);
	ver1.putstring.clearAll();
}

void VisionSystem::start()
{
	while (true)
	{	
		// print Webcam velocity  
		int fps = ver1.cap.get(CAP_PROP_FPS);
		ver1.putstring.os_webfps.name << "Webcam fps : " << fps;

		Mat img_input, img_hsv;
		Mat img_binary_team, img_binary_id1, img_binary_id2, img_binary_ball;

		// Get image from Cam
		if (!ver1.cap.read(img_input))
		{
			cout << "Can not find web camm...\n";
			break;
		}

		// RGB to HSV
		cvtColor(img_input, img_hsv, COLOR_BGR2HSV);

		// using HSV range, img to binary
		// team color
		inRange(img_hsv, Scalar(ver1.team.LowH, ver1.team.LowS, ver1.team.LowV),
			Scalar(ver1.team.HighH, ver1.team.HighS, ver1.team.HighV), img_binary_team);
		// id1 color
		inRange(img_hsv, Scalar(ver1.id1.LowH, ver1.id1.LowS, ver1.id1.LowV),
			Scalar(ver1.id1.HighH, ver1.id1.HighS, ver1.id1.HighV), img_binary_id1);
		// id2 color
		inRange(img_hsv, Scalar(ver1.id2.LowH, ver1.id2.LowS, ver1.id2.LowV),
			Scalar(ver1.id2.HighH, ver1.id2.HighS, ver1.id2.HighV), img_binary_id2);
		// ball color
		inRange(img_hsv, Scalar(ver1.ball.LowH, ver1.ball.LowS, ver1.ball.LowV),
			Scalar(ver1.ball.HighH, ver1.ball.HighS, ver1.ball.HighV), img_binary_ball);

		// morphological opening - remove small dot ..
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_ball, img_binary_ball, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_ball, img_binary_ball, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// morphological closing - put dot ..
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_ball, img_binary_ball, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_ball, img_binary_ball, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		
		// labeling img 
		Mat img_labels, centroids;
		Mat team_stats, id1_stats, id2_stats, ball_stats;
		ver1.team.numOfLables = connectedComponentsWithStats(img_binary_team, img_labels,
			team_stats, centroids, 8, CV_32S);
		ver1.id1.numOfLables = connectedComponentsWithStats(img_binary_id1, img_labels,
			id1_stats, centroids, 8, CV_32S);
		ver1.id2.numOfLables = connectedComponentsWithStats(img_binary_id2, img_labels,
			id2_stats, centroids, 8, CV_32S);
		ver1.ball.numOfLables = connectedComponentsWithStats(img_binary_ball, img_labels,
			ball_stats, centroids, 8, CV_32S);

		// Draw Area box
		drawAreaBox(img_input, team_stats, ver1.team.numOfLables, "team");
		drawAreaBox(img_input, id1_stats, ver1.id1.numOfLables, "id1");
		drawAreaBox(img_input, id2_stats, ver1.id2.numOfLables, "id2");
		drawAreaBox(img_input, ball_stats, ver1.ball.numOfLables, "ball");
		
		// Calculate !
		calculateTheLine(img_input ,x_id1, y_id1, x_id2, y_id2);
		
		// Draw Text on Frame
		drawText(img_input);
		
		//imshow("binary image", team.img_binary);
		imshow("Origin IMAGE", img_input);

		// Exit to ESC key
		if (waitKey(1) == 27) break;
	}
}

int main()
{
	VisionSystem vs = VisionSystem();
	if (!ver1.cap.isOpened())
	{
		cout << "Can not open Cam" << endl;
		return -1;
	}
	vs.start();
	return 0;
}