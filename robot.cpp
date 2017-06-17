//원래있던 코드 수정 - 로봇구조체에 실시간 저장
Robot r1;
void VisionSystem::calculateTheLine(Mat& img_input, double x1, double y1, double x2, double y2)
{
	r1._angle = angle(x1, y1, x2, y2);
	r1._x = Cx;
	r1._y = Cy;
	//이 과정 실시간으로 계속 - 로봇의 현재 위치 계속 가져오기!
}
//여기부터 새코드
//로봇구조체 선언부
typedef struct
{
	int _angle;
	int _x;
	int _y;
} Robot;

void Velocity ( Robot *robot, int vl, int vr )
{
	//Put your velocity-data-transfer between vision and arduino codes in here
	//아두이노로 신호 전송하는 코드.
}


// robot soccer system p329
//Robot * robot 은 int나 다른 타입으로 변경 가능
void Angle ( Robot *robot, int desired_angle)
{
	int theta_e, vl, vr;
	theta_e = desired_angle - (int)robot->_angle;

	while (theta_e > 180) theta_e -= 360;
	while (theta_e < -180) theta_e += 360;

	if (theta_e < -90) theta_e += 180;

	else if (theta_e > 90) theta_e -= 180;

	if (abs(theta_e) > 50)
	{
		vl = (int)(-9./90.0 * (double) theta_e);
		vr = (int)(9./90.0 * (double)theta_e);
	}
	else if (abs(theta_e) > 20)
	{
		vl = (int)(-11.0/90.0 * (double)theta_e);
		vr = (int)(11.0/90.0 * (double)theta_e);
	}
	else
	{
		vl = (int)(-13.0/90.0 * (double)theta_e);
		vr = (int)(13.0/90.0 * (double)theta_e);
	}
	Velocity (robot, vl, vr);//계산된 회전해야하는 앵글값대로 바퀴조정 ->아두이노로 보내는 함수 호출!
}

void Position( Robot *robot, double x, double y )
{
	int desired_angle = 0, theta_e = 0, d_angle = 0, vl, vr, vc = 125;

	double dx, dy, d_e, Ka = 10.0/90.0;
	dx = x - robot->_x;
	dy = y - robot->_y;

	d_e = sqrt(dx * dx + dy * dy);
	if (dx == 0 && dy == 0)
		desired_angle = 90;
	else
		desired_angle = (int)(180. / PI * atan2((double)(dy), (double)(dx)));
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
		vr = (int)( vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) + Ka * theta_e);
		vl = (int)( vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) - Ka * theta_e);
	}

	else
	{
		vr = (int)(+.17 * theta_e);
		vl = (int)(-.17 * theta_e);
	}

	if(robot._x < x + 10 && robot._y < y +10)
	{
		destinated = true;
		Velocity(robot, 0, 0);
	}// txt로 짰던 코드예외부분
	else
	{
		Velocity(robot, vl, vr);//아두이노로 보내는함수!
	}

}

bool destinated = false;
void Position_Angle(Robot *robot, double x, double y, int desired_angle)
{
	destinated = false;
	Position(robot, x, y);
	if (destinated)
	{
		Angle(robot, desired_angle);
	}
}
int width = 1024;
int height = 1278;//temporary value

void calculatingSize(double x, double y)
{
	//x,y 로봇현재위치값 (640, 480기준)을 width, height변수값 기준의 좌표로 만들어줌
	double realX = width * x / 640;
	double realY = height * y / 480;

	//실제로는 이 값을  Velocity - 아두이노 보내는 부분으로 보내줘야함!
	//아니면 Velocity - 아두이노 보내는 부분 함수 - 에서
	//이 함수를 한 번 호출해서 값 변경덮어쓰기 한 다음 보내면 될듯
}
