#include "main.h"

VisionSystem::VisionSystem()
{
	ver1.SetVideoSize(640, 480);
	ver1.SetRobotColors();
	ver1.SetUI("[Setup] Team, ID1, ID2, Ball", &ver1.team, &ver1.id1, &ver1.id2, &ver1.ball);
	cout << "VisionSystem version 2017.06.17 2:24AM \n";
}

VisionSystem::~VisionSystem()
{
}

void VisionSystem::setFourSides()
{
	// Set N side
	n1._x = 320 + 60;
	n1._y = 120;
	n2._x = 160;
	n2._y = 120;
	n3._x = 160;
	n3._y = 240 + 120;
	n4._x = 320 + 60;
	n4._y = 240 + 120;
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
	line(img_input, cvPoint(Rx, Ry), cvPoint(Cx, Cy), CV_RGB(0, 0, 0), 3, 8, 0);	   // 占쏙옙占썩선
	line(img_input, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(255, 255, 255), 1, 8, 0); // 占쌩심쇽옙
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

void VisionSystem::rtnRobotsDirection(double x, double y)
{
	// 회전식
	x = x - Cx;
	y = y - Cy;
	Rx = (x)*cos(theta) - (y)*sin(theta);
	Ry = (y)*cos(theta) + (x)*sin(theta);

	Rx += Cx;
	Ry += Cy;

	ver1.putstring.os_yaxb.name << "connected line within two idcolors: y = " << a << "x + " << b;

	a2 = double(Ry - Cy) / (Rx - Cx);
	b2 = Ry - a2*Rx;
	ver1.putstring.os_ya2b2.name << "direction line: " << "y = " << a2 << "x + " << b2;
}

int VisionSystem::angle(float x1, float y1, float x2, float y2)
{
	float angle = atan2(y2 - y1, x2 - x1) * 180 / 3.1415;
	if (0 < -angle - 45 && -angle - 45 < 180)
	{
		return -angle - 45;
	}
	else return 360 - angle - 45;
}

void VisionSystem::Velocity(Robot *robot, int vl, int vr)
{
	//Put your velocity-data-transfer between vision and arduino codes in here
	//아두이노로 신호 전송하는 코드.
	string send = to_string((double)vl/(double)25.5) + "/" + to_string((double)vr/(double)25.5);
	vector<char> writable(send.begin(), send.end());
	writable.push_back('\0');
	char* ptr = &writable[0];
	cout << "Send Value : " << ptr << "\n";
	if (!serialComm.sendCommand(ptr))
	{
		cout << "send command failed" << endl;
	}
	else cout << "send Command success" << endl;
}

void VisionSystem::RobotAngle(Robot *robot, int desired_angle)
{
	cout << "RobotAngle() \n";
	int theta_e, vl, vr;
	theta_e = desired_angle - (int)robot->_angle;

	while (theta_e > 180) theta_e -= 360;
	while (theta_e < -180) theta_e += 360;

	if (theta_e < -90) theta_e += 180;

	else if (theta_e > 90) theta_e -= 180;

	if (abs(theta_e) > 50)
	{
		vl = (int)(-9. / 90.0 * (double)theta_e);
		vr = (int)(9. / 90.0 * (double)theta_e);
	}
	else if (abs(theta_e) > 20)
	{
		vl = (int)(-11.0 / 90.0 * (double)theta_e);
		vr = (int)(11.0 / 90.0 * (double)theta_e);
	}
	else
	{
		vl = (int)(-13.0 / 90.0 * (double)theta_e);
		vr = (int)(13.0 / 90.0 * (double)theta_e);
	}
	Velocity(robot, vl, vr);//계산된 회전해야하는 앵글값대로 바퀴조정 ->아두이노로 보내는 함수 호출!
	
	realtimeCheck.vl = vl;
	realtimeCheck.vr = vr;
	
	if (!realtimeCheck.isPositionAngleCall) {
		realtimeCheck.isRobotOutOfBound = false;
		realtimeCheck.isPositionAngleCall = false;
		realtimeCheck.isPositionCall = false;
		realtimeCheck.isRobotAngleCall = true;
	}
}

void VisionSystem::Position(Robot *robot, double x, double y)
{
	cout << "Position() \n";
	int desired_angle = 0,
		theta_e = 0,
		d_angle = 0,
		vl, vr,
		vc = 125;

	double dx, dy, d_e, Ka = 10.0 / 90.0;
	dx = x - robot->_x;
	dy = y - robot->_y;

	d_e = sqrt(dx * dx + dy * dy);
	if (dx == 0 && dy == 0)
		desired_angle = 90;
	else
		desired_angle = (int)(180. / CV_PI * atan2((double)(dy), (double)(dx)));
	theta_e = desired_angle - (int)robot->_angle;

	while (theta_e > 180) theta_e -= 360;
	while (theta_e < -180) theta_e += 360;

	if (d_e > 100.)
		Ka = 17. / 90.;
	else if (d_e > 50)
		Ka = 19. / 90.;
	else if (d_e > 30)
		Ka = 21. / 90.;
	else if (d_e > 20)
		Ka = 23. / 90.;
	else
		Ka = 25. / 90.;

	if (theta_e > 95 || theta_e < -95)
	{
		theta_e += 180;

		if (theta_e > 180)
			theta_e -= 360;
		if (theta_e > 80)
			theta_e = 80;
		if (theta_e < -80)
			theta_e = -80;
		if (d_e < 5.0 && abs(theta_e) < 40)
			Ka = 0.1;
		vr = (int)(-vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) + Ka * theta_e);
		vl = (int)(-vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) - Ka * theta_e);
	}

	else if (theta_e < 85 && theta_e > -85)
	{
		if (d_e < 5.0 && abs(theta_e) < 40)
			Ka = 0.1;
		vr = (int)(vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) + Ka * theta_e);
		vl = (int)(vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) - Ka * theta_e);
	}

	else
	{
		vr = (int)(+.17 * theta_e);
		vl = (int)(-.17 * theta_e);
	}

	if (robot->_x < x + 10 && robot->_y < y + 10)
	{
		destinated = true;
		Velocity(robot, 0, 0);
	} 
	else if (robot->_x >= width - 30 || robot->_y >= height - 30)// txt로 짰던 코드예외부분
	{
		Velocity(robot, 0, 0);//일단멈춤
							  //그 이후 어케할지는 알아서 생각 (흑흑)
		realtimeCheck.vl = 0;
		realtimeCheck.vr = 0;
		realtimeCheck.isRobotOutOfBound = true;
		realtimeCheck.isPositionAngleCall = false;
		realtimeCheck.isPositionCall = false;
		realtimeCheck.isRobotAngleCall = false;
	}
	else
	{
		realtimeCheck.vl = vl;
		realtimeCheck.vr = vr;

		if (!realtimeCheck.isPositionAngleCall) {
			realtimeCheck.isRobotOutOfBound = false;
			realtimeCheck.isPositionAngleCall = false;
			realtimeCheck.isPositionCall = true;
			realtimeCheck.isRobotAngleCall = false;
		}

		Velocity(robot, vl, vr);//아두이노로 보내는함수!
	}

}

void VisionSystem::Position_Angle(Robot *robot, double x, double y, int desired_angle)
{
	cout << "Call Position_Angle()\n";
	destinated = false;
	Position(robot, x, y);
	if (destinated)
	{
		RobotAngle(robot, desired_angle);
	}
	realtimeCheck.isRobotOutOfBound = false;
	realtimeCheck.isPositionAngleCall = true;
	realtimeCheck.isPositionCall = false;
	realtimeCheck.isRobotAngleCall = false;
}

void VisionSystem::calculatingSize(double x, double y)
{
	//x,y 로봇현재위치값 (640, 480기준)을 width, height변수값 기준의 좌표로 만들어줌
	double realX = width * x / 640;
	double realY = height * y / 480;

	//실제로는 이 값을  Velocity - 아두이노 보내는 부분으로 보내줘야함!
	//아니면 Velocity - 아두이노 보내는 부분 함수 - 에서
	//이 함수를 한 번 호출해서 값 변경덮어쓰기 한 다음 보내면 될듯
}

void VisionSystem::calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2)
{
	makeLine(x1, y1, x2, y2);
	findCenterPoint(x1, y1, x2, y2);
	rtnRobotsDirection(x1, y1);
	ver1.putstring.os_angle.name << "angle: " << angle(x1, y1, x2, y2);
	drawLine(img_input, x1, y1, x2, y2);

	r1._angle = angle(x1, y1, x2, y2);
	r1._x = Cx;
	r1._y = Cy;

	ver1.putstring.os_cxcy.name << "Robot: (" << Cx << ", " << Cy << ")";
}

void VisionSystem::drawText(Mat& img_input)
{
	ver1.putstring.os_id1.name << "id1 (" << x_id1 << ", " << y_id1 << ")";
	ver1.putstring.os_id2.name << "id2 (" << x_id2 << ", " << y_id2 << ")";
	ver1.putstring.os_ball.name << "ball (" << x_ball << ", " << y_ball << ")";
	ver1.putstring.os_vrvl.name << "Velocity: (L:" << realtimeCheck.vl << " , R:" << realtimeCheck.vr << ")";
	whereisrobot(Cx, Cy); // Show where is robot

	// start (10, 320)
	ver1.putstring.setTextPosition(10, 320);
	ver1.putstring.onFrame(img_input);
	ver1.putstring.clearAll();
}

void VisionSystem::whereisrobot(int x, int y)
{
	/* Find Robot' current position and assign N side: Place
	@param x,y 로봇의 현재 중점좌표
	*/
	int place;

	if (x>320 && y>240) place = 4;
	else if (x <= 320 && y >240) place = 3;
	else if (x > 320 && y <= 240) place = 1;
	else if (x <= 320 && y <= 240) place = 2;
	else place = 0; // 0이면 err. (경기장밖)

	ver1.putstring.os_place.name << "N=" << place;
}

void VisionSystem::xyMode()
{
	int x, y, angle;
	int res = NULL;
	cout << "Input X, Y Position AND Angle, For Robot Move : ";
	res = scanf("x:%d, y:%d, angle:%d", &xyMode_x, &xyMode_y, &xyMode_angle);

	if (res == 3) {
		//x, y, angle 범위 처리(경기장 크기와 각도 제한)
		if ((x < 0 || x>320) && (y < 0 || y>240) && (angle<0 || angle>360)) {
			cout << "Out Of Range \n";
			Velocity(&r1, 0, 0);
			realtimeCheck.vl = 0;
			realtimeCheck.vr = 0;
			realtimeCheck.isRobotOutOfBound = true;
			realtimeCheck.isPositionAngleCall = false;
			realtimeCheck.isPositionCall = false;
			realtimeCheck.isRobotAngleCall = false;
		}
		else {
			cout << "ok Position_Angle Call\n";
			Position_Angle(&r1, xyMode_x, xyMode_y, xyMode_angle);
			return;
		}
	}
}

void VisionSystem::nSideToPosition(int whichPlace)
{

	if (whichPlace == 1 || whichPlace == 2 || whichPlace == 3 || whichPlace == 4)
	{

		// Call autoPosition for calculate angle, position
		// send robot vr, vl

		switch (whichPlace) {
		case 1:
			Position(&r1, n1._x, n1._y);
			return;
		case 2:
			Position(&r1, n2._x, n2._y);
			return;
		case 3:
			Position(&r1, n3._x, n3._y);
			return;
		case 4:
			Position(&r1, n4._x, n4._y);
			return;
		}
	}
}

void VisionSystem::NPlaceMode()
{
	// User Input for N sides
	int res = NULL;
	cout << "Input Number 1 ~ 4, For Robot Move :";
	res = scanf("%d", &whichPlace);
	if (res == 1) {
		cout << "ok Position Call\n";
		nSideToPosition(whichPlace);
	}
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
		if (waitKey(1) == 27) {
			serialComm.disconnect(); //작업이 끝나면 포트를 닫는다
			cout << "종료합니다 \n";
			break;
		}

		if (ans) { // 모드를 한번이상 입력했다면,
			if (!destinated) { // 그리고 도착도 안 한 상태면, 실시간 처리
				if (realtimeCheck.isPositionAngleCall)
					Position_Angle(&r1, xyMode_x, xyMode_y, xyMode_angle);
				if (realtimeCheck.isPositionCall)
					nSideToPosition(whichPlace); //Will Call Positon()
				if (realtimeCheck.isRobotAngleCall)
					RobotAngle(&r1, xyMode_angle);
				if (realtimeCheck.isRobotOutOfBound)
					Velocity(&r1, 0, 0);
			}
		}

		if ((char)waitKey(30) == ' ') { // space bar
			destinated = false;
			ans = false;
			cout << "로봇 모드 진입, 블루투스 확인중 \n";
			bluetooth = serialComm.connect("COM7");
			if (bluetooth)
			{
				cout << "connect successed" << endl;
				int mode_res = 0;

				while (mode_res != 1)
				{
					cout << "Choice Mode (XY Mode:1 or N Place Mode:2) : ";
					mode_res = scanf("%d", &mode);

					if (mode_res == 1) {
						ans = true;
						switch (mode)
						{
							case 1:
								xyMode(); //Will Call Position_Angle()
								break;
							case 2:
								NPlaceMode(); //Will Call Position()
								break;
							default:
								break;
						}
					}
					if (waitKey(1) == 27) return;
				}
			}
			else cout << "connect faliled" << endl; 
		}
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
	vs.setFourSides(); // N side
	vs.start();
	return 0;
}