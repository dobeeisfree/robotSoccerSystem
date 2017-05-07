#include "robot.h";

robot::robot()
{
}
robot::~robot()
{
}
void robot::SetH(int low, int high)
{
	LowH = low;
	HighH = high;
}
void robot::SetS(int low, int high)
{
	LowS = low;
	HighS = high;
}
void robot::SetV(int low, int high)
{
	LowV = low;
	HighV = high;
}