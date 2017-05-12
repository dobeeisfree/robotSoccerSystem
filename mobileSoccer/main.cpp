#include "main.h"

VisionSystem::VisionSystem()
{
	cout << "VisionSystem 2017.05.11 \n";
}

VisionSystem::~VisionSystem()
{
	cout << "Goodbye~~\n";
}

void VisionSystem::SetVideoSize(int width, int height)
{
	/*
	Set Video Size using width, height
	*/
	cap.set(CAP_PROP_FRAME_WIDTH, width);
	cap.set(CAP_PROP_FRAME_HEIGHT, height);
}

void VisionSystem::GenerateTrackbar(char * msg, object * color, char * TrackbarName)
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

void VisionSystem::SetUI(char * msg, object * teamColor, object * id1Color, object * id2Color, object * ballColor)
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

void VisionSystem::SetRobotColors()
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
	ball.SetH(170, 179);
	ball.SetS(50, 255);
	ball.SetV(0, 255);
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
	//realtime_decting << title << ",";
	//os_cc_stats << "[" << title << "]" << "(left:" << left << ", top:" << top << ", width:" << width << ", height:" << height << ")";
	
	if (strcmp(title, "id1") == 0)
	{
		x_one = round((double)(left + (double)(width / 2)));
		y_one = round((double)(top + (double)(height / 2)));
	}
	else if (strcmp(title, "id2") == 0)
	{
		x_two = round((double)(left + (double)(width / 2)));
		y_two = round((double)(top + (double)(height / 2)));
	}
	else if (strcmp(title, "ball") == 0)
	{
		x_ball = round((double)(left + (double)(width / 2)));
		y_ball = round((double)(top + (double)(height / 2)));
	}

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
	line(img_input, cvPoint(Rx, Ry), cvPoint(Cx, Cy), CV_RGB(255, 0, 0), 1, 8, 0);
	line(img_input, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(255, 255, 255), 1, 8, 0);
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
double VisionSystem::returnY(double x)
{
	return a*x + b;
}

void VisionSystem::findCenterPoint(double x1, double y1, double x2, double y2)
{
	Cx = (x1 + x2) / 2;
	Cy = (y1 + y2) / 2; //find robot's center point
}

void VisionSystem::makeTheta(double x1, double y1, double x2, double y2)
{
	//theta = atan((y1 - y2) / (x1 - x2)) - (3.1415f / 4);
	theta = (CV_PI / 4);
	//theta = 20;
}

void VisionSystem::rtnRobotsDirection(double x, double y)
{
	//회전식: 
	Rx = (x)*cos(theta) - (y)*sin(theta);
	Ry = (y)*cos(theta) + (x)*sin(theta);
	//좌표의 형태 : (x, returnY(x))
	os_yaxb << "connected line within two idcolors: y = " << a << "x + " << b;

	a2 = double(Ry - Cy) / (Rx - Cx);
	b2 = Ry - a2*Rx;
	os_ya2b2 << "direction line: " << "y = " << a2 << "x + " << b2;
}


void VisionSystem::calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2)
{
	makeLine(x1, y1, x2, y2);
	findCenterPoint(x1, y1, x2, y2);
	makeTheta(x1, y1, x2, y2);
	//for (int i = 0; i < 100; i++)
	//{
	rtnRobotsDirection(x1, y1);
	//}
	drawLine(img_input, x1, y1, x2, y2);
}

void VisionSystem::drawText(Mat& img_input)
{
	os_id1 << "id1 (" << x_one << ", " << y_one << ")";
	os_id2 << "id2 (" << x_two << ", " << y_two << ")";
	os_ball << "ball (" << x_ball << ", " << y_ball << ")";

	Point textOrg1(10, 320);
	Point textOrg2(10, 340);
	Point textOrg3(10, 360);
	Point textOrg4(10, 380);
	Point textOrg5(10, 400);
	Point textOrg6(10, 420);
	Point textOrg7(10, 440);

	const int fontFace = CV_FONT_HERSHEY_SIMPLEX;
	const double fontScale = 0.5; 
	const int thickness = 1;
	
	putText(img_input, os_webfps.str(), textOrg1, fontFace, fontScale, Scalar::all(255), thickness, 8);
	//putText(img_input, realtime_decting.str(), textOrg2, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(img_input, os_id1.str(), textOrg3, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(img_input, os_id2.str(), textOrg4, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(img_input, os_ball.str(), textOrg5, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(img_input, os_yaxb.str(), textOrg6, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(img_input, os_ya2b2.str(), textOrg7, fontFace, fontScale, Scalar::all(255), thickness, 8);
	
	os_webfps.str("");
	//realtime_decting.str("");
	os_id1.str("");
	os_id2.str("");
	os_yaxb.str("");
	os_ya2b2.str("");
	os_ball.str("");
}

void VisionSystem::VisionStart()
{
	while (true)
	{	
		// print Webcam velocity  
		int fps = cap.get(CAP_PROP_FPS);
		os_webfps << "Webcam fps : " << fps;

		Mat img_input, img_hsv;
		Mat img_binary_team, img_binary_id1, img_binary_id2, img_binary_ball;

		// Get image from Cam
		if (!cap.read(img_input))
		{
			cout << "Can not find web camm...\n";
			break;
		}

		// RGB to HSV
		cvtColor(img_input, img_hsv, COLOR_BGR2HSV);

		// using HSV range, img to binary
		// team color
		inRange(img_hsv, Scalar(team.LowH, team.LowS, team.LowV),
			Scalar(team.HighH, team.HighS, team.HighV), img_binary_team);
		// id1 color
		inRange(img_hsv, Scalar(id1.LowH, id1.LowS, id1.LowV),
			Scalar(id1.HighH, id1.HighS, id1.HighV), img_binary_id1);
		// id2 color
		inRange(img_hsv, Scalar(id2.LowH, id2.LowS, id2.LowV),
			Scalar(id2.HighH, id2.HighS, id2.HighV), img_binary_id2);
		// ball color
		inRange(img_hsv, Scalar(ball.LowH, ball.LowS, ball.LowV),
			Scalar(ball.HighH, ball.HighS, ball.HighV), img_binary_ball);

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
		team.numOfLables = connectedComponentsWithStats(img_binary_team, img_labels,
			team_stats, centroids, 8, CV_32S);
		id1.numOfLables = connectedComponentsWithStats(img_binary_id1, img_labels,
			id1_stats, centroids, 8, CV_32S);
		id2.numOfLables = connectedComponentsWithStats(img_binary_id2, img_labels,
			id2_stats, centroids, 8, CV_32S);
		ball.numOfLables = connectedComponentsWithStats(img_binary_ball, img_labels,
			ball_stats, centroids, 8, CV_32S);

		// Draw Area box
		drawAreaBox(img_input, team_stats, team.numOfLables, "team");
		drawAreaBox(img_input, id1_stats, id1.numOfLables, "id1");
		drawAreaBox(img_input, id2_stats, id2.numOfLables, "id2");
		drawAreaBox(img_input, ball_stats, ball.numOfLables, "ball");
		
		// Calculate !
		calculateTheLine(img_input ,x_one, y_one, x_two, y_two);
		
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
	vs.SetVideoSize(640, 480);
	if (!cap.isOpened())
	{
		cout << "Can not open Cam" << endl;
		return -1;
	}
	vs.SetRobotColors();
	vs.SetUI("[Setup] Team, ID1, ID2, Ball", &team, &id1, &id2, &ball);
	vs.VisionStart();
	return 0;
}