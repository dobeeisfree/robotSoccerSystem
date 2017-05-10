#include "main.h"

VisionSystem::VisionSystem()
{
}

VisionSystem::~VisionSystem()
{
}

void VisionSystem::SetVideoSize(int width, int height)
{
	/*
	Set Video Size using width, height
	*/
	cap.set(CAP_PROP_FRAME_WIDTH, width);
	cap.set(CAP_PROP_FRAME_HEIGHT, height);
}

void VisionSystem::GenerateTrackbar(char * msg, robot * color, char * TrackbarName)
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

void VisionSystem::SetUI(char * msg, robot * teamColor, robot * id1Color, robot * id2Color)
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
	printf("[%s] (%d, %d, %d, %d)\n", title, left, top, width, height);

	if (strcmp(title, "id1") == 0)
	{
		x_one = round((double)(left + (double)(width / 2)));
		y_one = round((double)(top + (double)(height / 2)));
		printf("[%s] round:(%lf, %lf)\n", title, x_one, y_one);
	}
	else if (strcmp(title, "id2") == 0)
	{
		x_two = round((double)(left + (double)(width / 2)));
		y_two = round((double)(top + (double)(height / 2)));
		printf("[%s] round:(%lf, %lf)\n", title, x_two, y_two);
	}

	rectangle(img_input, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 1);
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
	theta = atan(tan((y1 - y2) / (x1 - x2) - (3.1415f / 4)));
}

void VisionSystem::rtnRobotsDirection(double x)
{
	//회전식: 
	Rx = (x - Cx)*cos(theta) - (returnY(x) - Cy)*sin(theta);
	Ry = (returnY(x) - Cy)*cos(theta) + (x - Cx)*sin(theta);
	//좌표의 형태 : (x, returnY(x))
	cout << "두 점 사이를 이은 식: " << "y =" << a << "x+" << b << endl;

	a2 = double(Ry - Cy) / (Rx - Cx);
	b2 = Ry - a2*Rx;

	cout << "로봇의 방향을 가리키는 직선: " << "y =" << a2 << "x+" << b2 << endl;
}


void VisionSystem::calculateTheLine(double x1, double y1, double x2, double y2)
{
	printf("each CenterPoint : (%lf, %lf), (%lf, %lf)\n", x1, y1, x2, y2);

	makeLine(x1, y1, x2, y2);
	findCenterPoint(x1, y1, x2, y2);
	makeTheta(x1, y1, x2, y2);
	//for (int i = 0; i < 100; i++)
	//{
	rtnRobotsDirection(x1);
	//}
}

void VisionSystem::VisionStart()
{
	while (true)
	{
		// 생각해보자 콘솔유아이
		system("cls");

		// print Webcam velocity  
		int fps = cap.get(CAP_PROP_FPS);
		cout << "Webcam fps:" << fps << endl;

		Mat img_input, img_hsv;
		Mat img_binary_team, img_binary_id1, img_binary_id2;

		// Get image from Cam
		if (!cap.read(img_input))
		{
			cout << "Can not find web camm..." << endl;
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

		// morphological opening - remove small dot ..
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// morphological closing - put dot ..
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// labeling img 
		Mat img_labels, centroids;
		Mat team_stats, id1_stats, id2_stats;
		team.numOfLables = connectedComponentsWithStats(img_binary_team, img_labels,
			team_stats, centroids, 8, CV_32S);
		id1.numOfLables = connectedComponentsWithStats(img_binary_id1, img_labels,
			id1_stats, centroids, 8, CV_32S);
		id2.numOfLables = connectedComponentsWithStats(img_binary_id2, img_labels,
			id2_stats, centroids, 8, CV_32S);

		// Draw Area box
		drawAreaBox(img_input, team_stats, team.numOfLables, "team");
		drawAreaBox(img_input, id1_stats, id1.numOfLables, "id1");
		drawAreaBox(img_input, id2_stats, id2.numOfLables, "id2");
		calculateTheLine(x_one, y_one, x_two, y_two);

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
	vs.SetUI("Team, ID1, ID2", &team, &id1, &id2);
	vs.VisionStart();
	return 0;
}