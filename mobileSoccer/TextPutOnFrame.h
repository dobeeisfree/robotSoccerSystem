#pragma once
#include <sstream>			   // ostringstream
#include <opencv2/core/mat.hpp>	   // Mat
#include <opencv2/core/types.hpp> // Scalar, Point
#include <opencv2/imgproc.hpp>    // putText()

using std::ostringstream;
using cv::Mat;
using cv::Scalar;
using cv::Point;
/*
	To putText on Frame (using opencv)
	** realtime text draw **
*/
typedef struct ostring_ {
	ostringstream name;	// text !
	Point textOrg;		// text position
} str_stream;

class TextPutOnFrame
{
public:
	str_stream os_webfps;
	//str_stream realtime_decting;
	str_stream os_id1;
	str_stream os_id2;
	str_stream os_yaxb;
	str_stream os_ya2b2;
	str_stream os_ball;

	TextPutOnFrame();
	~TextPutOnFrame();

	void clearAll();
	void setTextPosition(int x, int y);
	void onFrame(Mat& img_input);
};