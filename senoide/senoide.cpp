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

double foo(double x) {
	return 3*(x+1)*x*(x-1);
}

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

	Graph g(-10, 10, -2.5, 2.5, "xres = 1000, yres = 500");

	g.drawAxis(true, 0, false, "full_ticks = false");
	g.drawAxis(false, 0, false, "ticks_to_left=false, full_ticks = true");
	g.drawFunc(sin, cv::Vec3b(0,128,255));
	g.drawFunc([](double x){return x*x;}, cv::Vec3b(200,200,50));

	g.write("../../output1.png");

	return 0;
}