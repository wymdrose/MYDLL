#include "../../lib/tubeknn.h"

TubeKnn::TubeKnn()
{

}

TubeKnn::~TubeKnn()
{

}


bool IsAllWhite(Mat inputmat)
{
	if (inputmat.rows * inputmat.cols == cvCountNonZero(&(IplImage)inputmat))
		return true;
	else
		return false;
}


cv::Mat thinImage(const cv::Mat & src, const int maxIterations = -1)
{
	assert(src.type() == CV_8UC1);
	cv::Mat dst;
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	int count = 0;  //记录迭代次数
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达
			break;
		std::vector<uchar *> mFlag; //用于标记需要删除的点
		//对点标记
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记
				//  p9 p2 p3
				//  p8 p1 p4
				//  p7 p6 p5
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
					{
						//标记
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空
		}

		//对点标记
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记
				//  p9 p2 p3
				//  p8 p1 p4
				//  p7 p6 p5
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
					{
						//标记
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空
		}
	}
	return dst;
}

bool cmp(const Rect& a, const Rect& b)
{
	if (a.x < b.x)
		return true;
	else
		return false;
}

bool TubeKnn::getTubeValue(Mat roi, QString& result, float anchorX, float anchorY, double thresh){

	cvtColor(roi, roi, CV_RGB2GRAY);

	GaussianBlur(roi, roi, Size(11, 11), 4, 4);
	imshow("gaussian blur", roi);

	Mat image_bin;
	threshold(roi, image_bin, thresh, 255, THRESH_BINARY);
	imshow("image_bin", image_bin);

	Mat image_dil;
	Mat element;

	element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
	morphologyEx(image_bin, image_dil, MORPH_DILATE, element);

	imshow("image_dil1", image_dil);

	//
	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy;
	findContours(image_dil, contours_out, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);  
	drawContours(image_dil, contours_out, -1, Scalar(255, 0, 0), 1);//  

	const size_t size = contours_out.size();
	vector<Rect> num_location;
	for (int i = 0; i < contours_out.size(); i++)
	{
		num_location.push_back(boundingRect(Mat(contours_out[i])));
	}
	sort(num_location.begin(), num_location.end(), cmp);

	char filenamew[255];
	int tube_num = 0;
	vector<Mat> tube;
	for (int i = 0; i < contours_out.size(); i++)
	{
		if (!IsAllWhite(image_dil(num_location.at(i))))
		{
			tube.push_back(image_bin(num_location.at(i)));
			sprintf(filenamew, "Snapshot/%s%d.jpg", "loaction", tube_num);
			imwrite(filenamew, tube.at(tube_num));
			tube_num++;
		}
	}
	cout << tube_num << endl;

	char trainfile[100];

	Mat traindata, trainlabel, tmp;
	for (int i = 0; i < 10; i++)
	{
		sprintf(trainfile, "%s\\%d.jpg", TRAINPATH, i);

		tmp = imread(trainfile, IMREAD_GRAYSCALE);
		threshold(tmp, tmp, 100, 255, THRESH_BINARY);
		cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
		traindata.push_back(tmp.reshape(0, 1));
		trainlabel.push_back(i);
	}

	/*
	for (int i = 0; i < 10; i++)
	{
		if (i == 2)
		{
			sprintf(trainfile, "%s\\%d.jpg", TRAINPATH, i);
		}
		else
		{
			sprintf(trainfile, "%s\\%d..jpg", TRAINPATH, i);
		}
		

		tmp = imread(trainfile, IMREAD_GRAYSCALE);
		threshold(tmp, tmp, 100, 255, THRESH_BINARY);
		cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
		traindata.push_back(tmp.reshape(0, 1));
		trainlabel.push_back(i + 20);
	}
	*/

	sprintf(trainfile, "%s\\E.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('E'));
	//
	/*
	sprintf(trainfile, "%s\\dot.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('D'));
	*/
	//
	traindata.convertTo(traindata, CV_32F);

	int K = 1;
	Ptr<TrainData> tData = TrainData::create(traindata, ROW_SAMPLE, trainlabel);
	Ptr<KNearest> knn = KNearest::create();
	knn->setDefaultK(K);
	knn->setIsClassifier(true);
	knn->train(tData);

	for (int i = 0; i < tube_num; i++)
	{
		cv::resize(tube.at(i), tube.at(i), Size(NORMWIDTH, NORMHEIGHT));
		tube.at(i) = tube.at(i).reshape(0, 1);
		tube.at(i).convertTo(tube.at(i), CV_32F);

		if (knn->predict(tube.at(i)) == 'E')
		{
			result += "E";
		}
		/*
		else if (knn->predict(tube.at(i)) == 'D')
		{
			result += ".";
		}
		else if (knn->predict(tube.at(i)) == 20)
		{
			result += "0.";
		}
		else if (knn->predict(tube.at(i)) == 21)
		{
			result += "1.";
		}
		else if (knn->predict(tube.at(i)) == 22)
		{
			result += "2.";
		}
		else if (knn->predict(tube.at(i)) == 23)
		{
			result += "3.";
		}
		else if (knn->predict(tube.at(i)) == 24)
		{
			result += "4.";
		}
		else if (knn->predict(tube.at(i)) == 25)
		{
			result += "5.";
		}
		else if (knn->predict(tube.at(i)) == 26)
		{
			result += "6.";
		}
		else if (knn->predict(tube.at(i)) == 27)
		{
			result += "7.";
		}
		else if (knn->predict(tube.at(i)) == 28)
		{
			result += "8.";
		}
		else if (knn->predict(tube.at(i)) == 29)
		{
			result += "9.";
		}*/
		else
		{
			auto a = QString::number(knn->predict(tube.at(i)));
			result += QString::number(knn->predict(tube.at(i)));
		}

	}

	waitKey(0);
	destroyAllWindows();

	return true;
}

bool TubeKnn::getTubeValue(QString pathTube, QString& result, float anchorX, float anchorY, double thresh)
{
	Mat image_org = imread(pathTube.toStdString(), IMREAD_COLOR);
	imshow("image_org", image_org);
	Mat image_gry = imread(pathTube.toStdString(), IMREAD_GRAYSCALE);
	if (image_gry.empty())
		return "empty";
	namedWindow("image_gry", WINDOW_AUTOSIZE);

	GaussianBlur(image_gry, image_gry, Size(11, 11), 4, 4);
	imshow("gaussian blur", image_gry);

		
	Mat image_bin;
	threshold(image_gry, image_bin, thresh, 255, THRESH_BINARY);
	imshow("image_bin", image_bin);

	Mat image_dil;
	Mat element;


	element = getStructuringElement(MORPH_ELLIPSE, Size(anchorX, anchorY));
	morphologyEx(image_bin, image_dil, MORPH_DILATE, element);

	imshow("image_dil1", image_dil);

	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy;
	findContours(image_dil, contours_out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);  // program crash when using debug
	drawContours(image_dil, contours_out, -1, Scalar(255, 0, 0), 1);//  

	// re-arrange loaction according to the real position in the original image 
	const size_t size = contours_out.size();
	vector<Rect> num_location;
	for (int i = 0; i < contours_out.size(); i++)
	{
		num_location.push_back(boundingRect(Mat(contours_out[i])));
	}
	sort(num_location.begin(), num_location.end(), cmp);

	// number extraction
	char filenamew[255];
	int tube_num = 0;
	vector<Mat> tube;
	for (int i = 0; i < contours_out.size(); i++)
	{
		if (!IsAllWhite(image_dil(num_location.at(i))))
		{
			tube.push_back(image_bin(num_location.at(i)));
			sprintf(filenamew, "Snapshot/%s%d.jpg", "loaction", tube_num);
			imshow(filenamew, tube.at(tube_num));
			imwrite(filenamew, tube.at(tube_num));
			tube_num++;
		}
	}
	cout << tube_num << endl;

	char trainfile[100];

	Mat traindata, trainlabel, tmp;
	for (int i = 0; i < 10; i++)
	{
		sprintf(trainfile, "%s\\%d.jpg", TRAINPATH, i);

		tmp = imread(trainfile, IMREAD_GRAYSCALE);
		threshold(tmp, tmp, 100, 255, THRESH_BINARY);
		cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
		traindata.push_back(tmp.reshape(0, 1));
		trainlabel.push_back(i);
	}

	sprintf(trainfile, "%s\\A.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('A'));
	//
	sprintf(trainfile, "%s\\E.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('E'));

	//
	sprintf(trainfile, "%s\\n.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('n'));
	//

	sprintf(trainfile, "%s\\d.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('d'));
	//
	sprintf(trainfile, "%s\\F.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('F'));
	//
	sprintf(trainfile, "%s\\b.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('b'));
	//
	sprintf(trainfile, "%s\\u.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('u'));
	//
	sprintf(trainfile, "%s\\L.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('L'));
	//
	sprintf(trainfile, "%s\\o.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('o'));

	//
	sprintf(trainfile, "%s\\C.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('C'));

	//
	sprintf(trainfile, "%s\\r.jpg", TRAINPATH);
	tmp = imread(trainfile, IMREAD_GRAYSCALE);
	threshold(tmp, tmp, 100, 255, THRESH_BINARY);
	cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
	traindata.push_back(tmp.reshape(0, 1));
	trainlabel.push_back(int('r'));
	//
	traindata.convertTo(traindata, CV_32F);

	int K = 1;
	Ptr<TrainData> tData = TrainData::create(traindata, ROW_SAMPLE, trainlabel);
	Ptr<KNearest> knn = KNearest::create();
	knn->setDefaultK(K);
	knn->setIsClassifier(true);
	knn->train(tData);

	for (int i = 0; i < tube_num; i++)
	{
		cv::resize(tube.at(i), tube.at(i), Size(NORMWIDTH, NORMHEIGHT));
		tube.at(i) = tube.at(i).reshape(0, 1);
		tube.at(i).convertTo(tube.at(i), CV_32F);

		if (knn->predict(tube.at(i)) == 'E')
		{
			result += "E";
		}
		else if (knn->predict(tube.at(i)) == 'A')
		{
			result += "A";
		}
		else if (knn->predict(tube.at(i)) == 'n')
		{
			result += "n";
		}
		else if (knn->predict(tube.at(i)) == 'd')
		{
			result += "d";
		}
		else if (knn->predict(tube.at(i)) == 'F')
		{
			result += "F";
		}
		else if (knn->predict(tube.at(i)) == 'b')
		{
			result += "b";
		}
		else if (knn->predict(tube.at(i)) == 'u')
		{
			result += "u";
		}
		else if (knn->predict(tube.at(i)) == 'L')
		{
			result += "L";
		}
		else if (knn->predict(tube.at(i)) == 'o')
		{
			result += "o";
		}
		else if (knn->predict(tube.at(i)) == 'C')
		{
			result += "C";
		}
		else if (knn->predict(tube.at(i)) == 'r')
		{
			result += "r";
		}
		else
		{
			auto a = QString::number(knn->predict(tube.at(i)));
			result += QString::number(knn->predict(tube.at(i)));
		}

	}

	waitKey(500);
	destroyAllWindows();

	return true;
}

/**
* @brief 对输入图像进行细化
* @param src为输入图像,用cvThreshold函数处理过的8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
* @param maxIterations限制迭代次数，如果不进行限制，默认为-1，代表不限制迭代次数，直到获得最终结果
* @return 为对src细化后的输出图像,格式与src格式相同，元素中只有0与1,1代表有元素，0代表为空白
*/
