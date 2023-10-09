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
	std::stringstream ss_img, ss_yml;
	cv::Mat image;

	ss_yml << "senoide-" << SIDE << ".yml";
	image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

	cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

	for (int i = 0; i < SIDE; i++) {
	for (int j = 0; j < SIDE; j++) {
		image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
	}
	}

	fs << "mat" << image;
	fs.release();

	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
	image.convertTo(image, CV_8U);
	ss_img << "senoide-" << SIDE << ".png";
	cv::imwrite(ss_img.str(), image);

	cv::Mat imageyaml;

	fs.open(ss_yml.str(), cv::FileStorage::READ);
	fs["mat"] >> imageyaml;

	cv::Mat imagepng = cv::imread("senoide-256.png", cv::IMREAD_GRAYSCALE);

	std::vector<double> x;
	for (int i = 0; i < 256; i++) {
		x.push_back(i);
	}

	std::vector<double> y_png = imagepng.row(0);
	std::vector<double> y_yaml = imageyaml.row(0);
	std::vector<double> y;
	for (int i = 0; i < 256; i++) {
		y.push_back(y_png[i] - y_yaml[i]);
	}

	Graph g(0, 255, -1.5, 1, "xres = 1000, yres = 500");

	g.drawAxis(true, 0, false);
	g.drawAxis(false, 0, false, "ticks_to_left=false");
	g.drawFunc(x, y, cv::Vec3b(0,0,255));

	g.write("output1.png");

	return 0;
}