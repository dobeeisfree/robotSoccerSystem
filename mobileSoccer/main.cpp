#include <iostream>  
#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;



int main()
{
	VideoCapture cap(0);

	//웹캡에서 캡처되는 이미지 크기를 320x240으로 지정  
	cap.set(CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CAP_PROP_FRAME_HEIGHT, 240);

	if (!cap.isOpened())
	{
		cout << "웹캠을 열 수 없습니다." << endl;
		return -1;
	}


	namedWindow("찾을 색범위 설정 Team", CV_WINDOW_AUTOSIZE);
	namedWindow("찾을 색범위 설정 ID1", CV_WINDOW_AUTOSIZE);
	namedWindow("찾을 색범위 설정 ID2", CV_WINDOW_AUTOSIZE);
	//트랙바에서 사용되는 변수 초기화 
	int LowH_team = 112;
	int HighH_team = 179;

	int LowS_team = 157;
	int HighS_team = 255;

	int LowV_team = 47;
	int HighV_team = 255;

	//트랙바에서 사용되는 변수 초기화 
	int LowH_id1 = 148;
	int HighH_id1 = 179;

	int LowS_id1 = 77;
	int HighS_id1 = 255;

	int LowV_id1 = 47;
	int HighV_id1 = 255;
	
	//트랙바에서 사용되는 변수 초기화 
	int LowH_id2 = 8;
	int HighH_id2 = 39;

	int LowS_id2 = 168;
	int HighS_id2 = 255;

	int LowV_id2 = 47;
	int HighV_id2 = 255;


	//트랙바 생성 
	cvCreateTrackbar("LowH_team", "찾을 색범위 설정 Team", &LowH_team, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH_team", "찾을 색범위 설정 Team", &HighH_team, 179);

	cvCreateTrackbar("LowS_team", "찾을 색범위 설정 Team", &LowS_team, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS_team", "찾을 색범위 설정 Team", &HighS_team, 255);

	cvCreateTrackbar("LowV_team", "찾을 색범위 설정 Team", &LowV_team, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV_team", "찾을 색범위 설정 Team", &HighV_team, 255);

	//트랙바 생성 
	cvCreateTrackbar("LowH_id1", "찾을 색범위 설정 ID1", &LowH_id1, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH_id1", "찾을 색범위 설정 ID1", &HighH_id1, 179);

	cvCreateTrackbar("LowS_id1", "찾을 색범위 설정 ID1", &LowS_id1, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS_id1", "찾을 색범위 설정 ID1", &HighS_id1, 255);

	cvCreateTrackbar("LowV_id1", "찾을 색범위 설정 ID1", &LowV_id1, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV_id1", "찾을 색범위 설정 ID1", &HighV_id1, 255);

	//트랙바 생성 
	cvCreateTrackbar("LowH_id2", "찾을 색범위 설정 ID2", &LowH_id2, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH_id2", "찾을 색범위 설정 ID2", &HighH_id2, 179);

	cvCreateTrackbar("LowS_id2", "찾을 색범위 설정 ID2", &LowS_id2, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS_id2", "찾을 색범위 설정 ID2", &HighS_id2, 255);

	cvCreateTrackbar("LowV_id2", "찾을 색범위 설정 ID2", &LowV_id2, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV_id2", "찾을 색범위 설정 ID2", &HighV_id2, 255);


	while (true)
	{

		//웹캠에서 캡처되는 속도 출력  
		int fps = cap.get(CAP_PROP_FPS);
		cout << fps << endl;

		Mat img_input, img_hsv, img_binary_team, img_binary_id1, img_binary_id2;


		//카메라로부터 이미지를 가져옴 
		bool ret = cap.read(img_input);

		if (!ret)
		{
			cout << "카메라로부터 이미지를 가져올 수 없습니다." << endl;
			break;
		}


		//HSV로 변환
		cvtColor(img_input, img_hsv, COLOR_BGR2HSV);


		//지정한 HSV 범위를 이용하여 영상을 이진화
		inRange(img_hsv, Scalar(LowH_team, LowS_team, LowV_team), Scalar(HighH_team, HighS_team, HighV_team), img_binary_team);
		inRange(img_hsv, Scalar(LowH_id1, LowS_id1, LowV_id1), Scalar(HighH_id1, HighS_id1, HighV_id1), img_binary_id1);
		inRange(img_hsv, Scalar(LowH_id2, LowS_id2, LowV_id2), Scalar(HighH_id2, HighS_id2, HighV_id2), img_binary_id2);

		//morphological opening 작은 점들을 제거 
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//morphological closing 영역의 구멍 메우기 
		dilate(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_team, img_binary_team, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id1, img_binary_id1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary_id2, img_binary_id2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//라벨링 
		cv::Mat img_labels, stats_team, stats_id1, stats_id2, centroids;
		int numOfLables_team = connectedComponentsWithStats(img_binary_team, img_labels,
			stats_team, centroids, 8, CV_32S);
		int numOfLables_id1 = connectedComponentsWithStats(img_binary_id1, img_labels,
			stats_id1, centroids, 8, CV_32S);
		int numOfLables_id2 = connectedComponentsWithStats(img_binary_id2, img_labels,
			stats_id2, centroids, 8, CV_32S);


		//영역박스 그리기
		int max_team = -1, idx_team = 0;
		int max_id1 = -1, idx_id1 = 0;
		int max_id2 = -1, idx_id2 = 0;
		for (int j = 1; j < numOfLables_team; j++) {
			int area_team = stats_team.at<int>(j, CC_STAT_AREA);
			if (max_team < area_team)
			{
				max_team = area_team;
				idx_team = j;
			}
		}
		for (int j = 1; j < numOfLables_id1; j++) {
			int area_id1 = stats_id1.at<int>(j, CC_STAT_AREA);
			if (max_id1 < area_id1)
			{
				max_id1 = area_id1;
				idx_id1 = j;
			}
		}for (int j = 1; j < numOfLables_id2; j++) {
			int area_id2 = stats_id2.at<int>(j, CC_STAT_AREA);
			if (max_id2 < area_id2)
			{
				max_id2 = area_id2;
				idx_id2 = j;
			}
		}


		int left_team = stats_team.at<int>(idx_team, CC_STAT_LEFT);
		int top_team = stats_team.at<int>(idx_team, CC_STAT_TOP);
		int width_team = stats_team.at<int>(idx_team, CC_STAT_WIDTH);
		int height_team = stats_team.at<int>(idx_team, CC_STAT_HEIGHT);

		int left_id1 = stats_id1.at<int>(idx_id1, CC_STAT_LEFT);
		int top_id1 = stats_id1.at<int>(idx_id1, CC_STAT_TOP);
		int width_id1 = stats_id1.at<int>(idx_id1, CC_STAT_WIDTH);
		int height_id1 = stats_id1.at<int>(idx_id1, CC_STAT_HEIGHT);

		int left_id2 = stats_id2.at<int>(idx_id2, CC_STAT_LEFT);
		int top_id2 = stats_id2.at<int>(idx_id2, CC_STAT_TOP);
		int width_id2 = stats_id2.at<int>(idx_id2, CC_STAT_WIDTH);
		int height_id2 = stats_id2.at<int>(idx_id2, CC_STAT_HEIGHT);


		rectangle(img_input, Point(left_team, top_team), Point(left_team + width_team, top_team + height_team),
			Scalar(0, 0, 255), 1);

		rectangle(img_input, Point(left_id1, top_id1), Point(left_id1 + width_id1, top_id1 + height_id1),
			Scalar(0, 0, 255), 1);

		rectangle(img_input, Point(left_id2, top_id2), Point(left_id2 + width_id2, top_id2 + height_id2),
			Scalar(0, 0, 255), 1);


		//imshow("이진화 영상", img_binary_team);
		imshow("원본 영상", img_input);


		//ESC키 누르면 프로그램 종료
		if (waitKey(1) == 27)
			break;
	}


	return 0;
}