#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <cmath>
#include "cpplot/headers/cpplot.hpp"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

int SIDE = 256;
int PERIODOS = 4;

int main(int argc, char** argv) {
	// std::stringstream ss_img, ss_yml;
	// cv::Mat image;

	// ss_yml << "senoide-" << SIDE << ".yml";
	// image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

	// cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

	// for (int i = 0; i < SIDE; i++) {
	// for (int j = 0; j < SIDE; j++) {
	// 	image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
	// }
	// }

	// fs << "mat" << image;
	// fs.release();

	// cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
	// image.convertTo(image, CV_8U);
	// ss_img << "senoide-" << SIDE << ".png";
	// cv::imwrite(ss_img.str(), image);

	// fs.open(ss_yml.str(), cv::FileStorage::READ);
	// fs["mat"] >> image;

	// return 0;

	Graph g(-1, 7, -2, 2, 1000, 500);

	g.drawFunc(sin, 0, 2*M_PI, -0.7, 0.7, 2, {0, 255, 255});
	
	g.write("../../output1.png");
	g.setRange(0.5, 20, -0.5, g.ymax());
	g.write("../../output2.png");
	g.setRes(1000, 250);
	g.write("../../output3.png");

	return 0;
}