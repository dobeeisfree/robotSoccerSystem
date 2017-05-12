#pragma once

class object
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

	object();
	~object();

	void SetH(int low, int high);
	void SetS(int low, int high);
	void SetV(int low, int high);
};