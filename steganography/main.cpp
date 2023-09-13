#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    
    if (argc <= 1 || argc > 2) {
        std::cout << "number of arguments should be exactly 1.\n";
        exit(1);
    }

    std::string img_path(argv[1]);
    img_path = "../../" + img_path;

    cv::Mat image;
    image = cv::imread(img_path, cv::IMREAD_COLOR);
    cv::Mat plane_image(2*image.rows, 4*image.cols, CV_8UC3);

    for (uchar k = 0; k < 8; k++) {
        int mask = 255 >> k;
        int dispx = k > 3 ? image.rows : 0;
        int dispy = k > 3 ? (k - 4)*image.cols : k*image.cols;

        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                plane_image.at<cv::Vec3b>(i + dispx,j + dispy)[0] = image.at<cv::Vec3b>(i,j)[0] << k;
                plane_image.at<cv::Vec3b>(i + dispx,j + dispy)[1] = image.at<cv::Vec3b>(i,j)[1] << k;
                plane_image.at<cv::Vec3b>(i + dispx,j + dispy)[2] = image.at<cv::Vec3b>(i,j)[2] << k;
            }
        }
    }

    cv::imwrite("../../output.png", plane_image);

    return 0;
}