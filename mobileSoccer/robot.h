#pragma once

class robot
{
public:

	// colors
	int LowH;
	int HighH;
	int LowS;
	int HighS;
	int LowV;
	int HighV;

	// Area
	int numOfLables;

	robot();
	~robot();

	void SetH(int low, int high);
	void SetS(int low, int high);
	void SetV(int low, int high);
};