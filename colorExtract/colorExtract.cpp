#include "../../lib/colorExtract.h"

using namespace OpencvApi;
using namespace std;
using namespace cv;

ColorExtract::ColorExtract()
{

}

ColorExtract::~ColorExtract()
{

}


void ColorExtract::get()
{
	Mat matSrc = imread("color.BMP", IMREAD_UNCHANGED);
	Mat matHsv;
	cvtColor(matSrc, matHsv, COLOR_BGR2HSV);

	vector<int> colorVec;
	colorVec.push_back(matHsv.at<uchar>(0, 0));
	colorVec.push_back(matHsv.at<uchar>(0, 1));
	colorVec.push_back(matHsv.at<uchar>(0, 2));

	if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 255)
		&& (colorVec[2] >= 0 && colorVec[2] <= 46)){

		cout << "ºÚ" << endl;
	}
	else if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 43)
		&& (colorVec[2] >= 46 && colorVec[2] <= 220)){

		cout << "»Ò" << endl;
	}
	else if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 30)
		&& (colorVec[2] >= 221 && colorVec[2] <= 255)){

		cout << "°×" << endl;
	}
	else if (((colorVec[0] >= 0 && colorVec[0] <= 10) || (colorVec[0] >= 156 && colorVec[0] <= 180))
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "ºì" << endl;

	}
	else if ((colorVec[0] >= 11 && colorVec[0] <= 25)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "³È" << endl;
	}
	else if ((colorVec[0] >= 26 && colorVec[0] <= 34)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "»Æ" << endl;
	}
	else if ((colorVec[0] >= 35 && colorVec[0] <= 77)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "ÂÌ" << endl;
	}
	else if ((colorVec[0] >= 78 && colorVec[0] <= 99)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "Çà" << endl;
	}
	else if ((colorVec[0] >= 100 && colorVec[0] <= 124)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "À¶" << endl;
	}
	else if ((colorVec[0] >= 125 && colorVec[0] <= 155)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)){

		cout << "×Ï" << endl;
	}
	else{

		cout << "Î´Öª" << endl;
	}

	getchar();


}

void ColorExtract::getOne(int HSV[], int flag)
{
	Mat imgOriginal = imread("cable.jpg", IMREAD_COLOR);

	int m = imgOriginal.rows;
	int n = imgOriginal.cols;
	Rect rect(0, 0, n*0.5, m * 1);
	Mat temp(imgOriginal, rect);
	namedWindow("roi");
	imshow("roi", temp);


	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	Mat imgThresholded;

	inRange(imgHSV, Scalar(HSV[0], HSV[2], HSV[4]), Scalar(HSV[1], HSV[3], HSV[5]), imgThresholded);
	imshow("Thresholded1", imgThresholded);

	Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
	if (flag)
	{
		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	}
	else
	{
		dilate(imgThresholded, imgThresholded, element);
	}

	imshow("Thresholded2", imgThresholded);
	imshow("Original", imgOriginal);

	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy;
	findContours(imgThresholded, contours_out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (size_t k = 0; k < contours_out.size(); k++)
	{
		Rect ret1 = boundingRect(Mat(contours_out[k]));

		int avgX = (ret1.x + ret1.x + ret1.width) / 2;

		int avgY = (ret1.y + ret1.y + ret1.height) / 2;

		cout << "x:" << avgX << "   y:" << avgY << endl;
	}

	cout << " ****** " << endl;

	waitKey(1);

}

bool ColorExtract::checkColor(int HSV[], Mat imgOriginal)
{
	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	Mat imgThresholded;

	inRange(imgHSV, Scalar(HSV[0], HSV[2], HSV[4]), Scalar(HSV[1], HSV[3], HSV[5]), imgThresholded);
	imshow("Thresholded1", imgThresholded);

	Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));

	morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	//dilate(imgThresholded, imgThresholded, element);

	imshow("Thresholded2", imgThresholded);
	imshow("Original", imgOriginal);

	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy;
	findContours(imgThresholded, contours_out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	waitKey(500);

	if (contours_out.size() > 0)
	{
		return true;
	}

	return false;
}

bool ColorExtract::checkColor(int HSV[], QString path){

	Mat imgOriginal = imread(path.toStdString(), IMREAD_COLOR);

	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	Mat imgThresholded;

	inRange(imgHSV, Scalar(HSV[0], HSV[2], HSV[4]), Scalar(HSV[1], HSV[3], HSV[5]), imgThresholded);
	imshow("Thresholded1", imgThresholded);

	Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));

	morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	//dilate(imgThresholded, imgThresholded, element);

	imshow("Thresholded2", imgThresholded);
	imshow("Original", imgOriginal);

	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy;
	findContours(imgThresholded, contours_out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	waitKey(1);

	if (contours_out.size() > 0)
	{
		return true;
	}

	return false;
}
