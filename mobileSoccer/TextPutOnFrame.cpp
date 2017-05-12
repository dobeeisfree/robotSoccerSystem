#include "TextPutOnFrame.h"

TextPutOnFrame::TextPutOnFrame()
{
}

TextPutOnFrame::~TextPutOnFrame()
{
}

void TextPutOnFrame::clearAll()
{
	os_webfps.name.str("");
	//realtime_decting.str("");
	os_id1.name.str("");
	os_id2.name.str("");
	os_yaxb.name.str("");
	os_ya2b2.name.str("");
	os_ball.name.str("");
}

void TextPutOnFrame::setTextPosition(int x, int y)
{
	os_webfps.textOrg.x = x;
	os_webfps.textOrg.y = y + 20;
	os_id1.textOrg.x = x;
	os_id1.textOrg.y = y + 40;
	os_id2.textOrg.x = x;
	os_id2.textOrg.y = y + 60;
	os_ball.textOrg.x = x;
	os_ball.textOrg.y = y + 80;
	os_yaxb.textOrg.x = x;
	os_yaxb.textOrg.y = y + 100;
	os_ya2b2.textOrg.x = x;
	os_ya2b2.textOrg.y = y + 120;
}

void TextPutOnFrame::onFrame(Mat& frame)
{
	const int fontFace = 0;
	const double fontScale = 0.5;
	const int thickness = 1;

	putText(frame, os_webfps.name.str(), os_webfps.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	//putText(frame, realtime_decting.str(), os_id1.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(frame, os_id1.name.str(), os_id1.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(frame, os_id2.name.str(), os_id2.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(frame, os_ball.name.str(), os_ball.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(frame, os_yaxb.name.str(), os_yaxb.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
	putText(frame, os_ya2b2.name.str(), os_ya2b2.textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
}