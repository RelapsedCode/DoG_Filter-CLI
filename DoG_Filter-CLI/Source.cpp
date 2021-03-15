#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;
using namespace cv;
Mat src, src_gray, src_grayS, dst, g1, g2, gradS, dog, th, sobel;

//Threshold
int threshold_value = 0;
int threshold_type = 0;
int g1s = 20, g2s = 4;

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}
	r += "C";
	r += (chans + '0');

	return r;
}

static void ProcessImage(int, void*)
{
	GaussianBlur(src_gray, g1, Size(0, 0), g1s, g1s);
	GaussianBlur(src_gray, g2, Size(0, 0), g2s, g2s);
	dog = g1 - g2;
	threshold(dog, th, threshold_value, 255, threshold_type);

	//Dont move it - th is not created
	Mat zerocross = th.clone();
	Mat srcContour = src.clone();

	//Zerocrossing
	for (int y = 1; y < th.rows - 1; y++) {
		for (int x = 1; x < th.cols - 1; x++) {
			for (int c = 0; c < th.channels(); c++) {
				zerocross.at<uchar>(y, x) =
					((th.at<uchar>(y, x) != th.at<uchar>(y + 1, x)) //bottom
						|| (th.at<uchar>(y, x) != th.at<uchar>(y, x + 1)) //right
						|| (th.at<uchar>(y, x) != th.at<uchar>(y + 1, x + 1)) //bottom & right
						|| (th.at<uchar>(y, x) != th.at<uchar>(y - 1, x)) // top
						|| (th.at<uchar>(y, x) != th.at<uchar>(y, x - 1)) // left
						|| (th.at<uchar>(y, x) != th.at<uchar>(y - 1, x - 1)) //top & left
						|| (th.at<uchar>(y, x) != th.at<uchar>(y - 1, x + 1)) // top & right
						|| (th.at<uchar>(y, x) != th.at<uchar>(y - 1, x - 1)) //bottom & left
						) ? 255 : 0;
			}
		}
	}

	//Original + Contour - If its white in the b&w image (we have a contour) its white in the srcContour. If its black it preserve its original color.
	for (int y = 1; y < zerocross.rows - 1; y++) {
		for (int x = 1; x < zerocross.cols - 1; x++) {
			for (int c = 0; c < zerocross.channels(); c++) {
				Vec3b& intensity = srcContour.at<Vec3b>(y, x);
				if (zerocross.at<uchar>(y, x) == 255) {
					for (int c = 0; c < srcContour.channels(); c++) {
						intensity.val[c] = 255; //255-white, 0-black
					}
				}
			}
		}
	}

	cout << "Count white pixels: " << countNonZero(zerocross) << endl;

	//namedWindow("Zero-crossing", WINDOW_NORMAL);
	//imshow("Zero-crossing", zerocross);
	string imageNameZ = "../outputData/Asparuh/Zero-crossing/ZC_Asparuh" + to_string(g1s) + "-" + to_string(g2s) + ".png";
	imwrite(imageNameZ, zerocross);
	cout << imageNameZ << endl;

	//namedWindow("Source+Contour-" + to_string(g1s) + "-" + to_string(g2s), WINDOW_NORMAL);
	//imshow("Source+Contour-" + to_string(g1s) + "-" + to_string(g2s), srcContour);
	string imageNameSC = "../outputData/Asparuh/Source+Contour/SC_Asparuh" + to_string(g1s) + "-" + to_string(g2s) + ".png";
	imwrite(imageNameSC, srcContour);
	cout << imageNameSC << endl;
}

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "../inputData/Asparuh.png";

	src = imread(filename, IMREAD_COLOR);
	if (src.empty())
	{
		printf(" Error opening image\n");
		printf(" Usage: ./Smoothing [image_name -- default /inputData/Asparuh.png] \n");
		return -1;
	}

	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	string ty = type2str(src_gray.type());
	printf("Matrix: %s %dx%d \n", ty.c_str(), src_gray.cols, src_gray.rows);

	//ProcessImage(0, 0);

	cout << "Enter sigma 1: "; cin >> g1s;
	cout << "Enter sigma 2: "; cin >> g2s;

	while (g1s <=40) {
		ProcessImage(0, 0);
		g1s += 5;
		g2s++;
	}

	char key = (char)waitKey(0);
	if (key == 27)
	{
		return 0;
	}
}


