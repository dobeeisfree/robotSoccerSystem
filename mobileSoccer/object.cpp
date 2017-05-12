#include "object.h";

object::object()
{
}
object::~object()
{
}
void object::SetH(int low, int high)
{
	LowH = low;
	HighH = high;
}
void object::SetS(int low, int high)
{
	LowS = low;
	HighS = high;
}
void object::SetV(int low, int high)
{
	LowV = low;
	HighV = high;
}