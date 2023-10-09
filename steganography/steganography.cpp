#include <iostream>
#include <opencv2/opencv.hpp>

#define DECODE 0
#define ENCODE 1
#define SHOW_PLANES 2

int main(int argc, char** argv) {
    
    std::string e_img_path;
    std::string h_img_path;
    uchar mode;

    //treat input arguments
    if (argc == 1) {
        std::cout << "no arguments passed.\n";
        return 0;
    } else if (argc == 2) {
        if (std::strcmp(argv[1], "encode") == 0) {
            std::cout << "correct syntax for encoding is \"stenography encode <path-to-encoding-image> <path-to-host-image>\".\n";
            return 0;
        } else if (std::strcmp(argv[1], "decode") == 0) {
            std::cout << "correct syntax for decoding is \"stenography decode <path-to-image>\".\n";
            return 0;
        } else if (std::strcmp(argv[1], "show_planes") == 0) {
            std::cout << "correct syntax for showing planes is \"stenography show_planes <path-to-image>\".\n";
            return 0;
        } else {
            std::cout << "valid options are \"encode\", \"decode\" or \"show_planes\".\n";
            return 0;
        }
    } else if (argc == 3) {
        if (std::strcmp(argv[1], "decode") == 0) {
            h_img_path = argv[2];
            mode = DECODE;
        } else if (std::strcmp(argv[1], "show_planes") == 0){
            h_img_path = argv[2];
            mode = SHOW_PLANES;
        } else if (std::strcmp(argv[1], "encode") == 0) {
            std::cout << "correct syntax for encoding is \"stenography encode <path-to-encoding-image> <path-to-host-image>\".\n";
            return 0;
        } else {
            std::cout << "valid options are \"encode\", \"decode\" and \"show_planes\".\n";
            return 0;
        }
    } else if (argc == 4) {
        if (std::strcmp(argv[1], "encode") == 0) {
            e_img_path = argv[2];
            h_img_path = argv[3];
            mode = ENCODE;
        } else if (std::strcmp(argv[1], "decode") == 0) {
            std::cout << "correct syntax for decoding is \"stenography decode <path-to-image>\".\n";
            return 0;
        } else if (std::strcmp(argv[1], "show_planes") == 0) {
            std::cout << "correct syntax for showing planes is \"stenography show_planes <path-to-image>\".\n";
            return 0;
        } else {
            std::cout << "valid options are \"encode\" and \"decode\".\n";
            return 0;
        }
    } else {
        std::cout << "invalid number of arguments.\n";
        return 0;
    }

    if (mode == ENCODE) {
        cv::Mat h_img, e_img;
        h_img = cv::imread(h_img_path, cv::IMREAD_COLOR);
        e_img = cv::imread(e_img_path, cv::IMREAD_COLOR);

        if (h_img.rows != e_img.rows || h_img.cols != e_img.cols) {
            std::cout << "both images must have the same dimensions.\n";
            return 0;
        }

        for (int i = 0; i < h_img.rows; i++) {
            for (int j = 0; j < h_img.cols; j++) {
                h_img.at<cv::Vec3b>(i, j)[0] = (h_img.at<cv::Vec3b>(i, j)[0] & 248) | (e_img.at<cv::Vec3b>(i, j)[0] >> 5);
                h_img.at<cv::Vec3b>(i, j)[1] = (h_img.at<cv::Vec3b>(i, j)[1] & 248) | (e_img.at<cv::Vec3b>(i, j)[1] >> 5);
                h_img.at<cv::Vec3b>(i, j)[2] = (h_img.at<cv::Vec3b>(i, j)[2] & 248) | (e_img.at<cv::Vec3b>(i, j)[2] >> 5);
            }
        }

        cv::imwrite("encoded_output.png", h_img);

    } else if (mode == DECODE) {
        
        cv::Mat h_img;
        h_img = cv::imread(h_img_path, cv::IMREAD_COLOR);

        for (int i = 0; i < h_img.rows; i++) {
            for (int j = 0; j < h_img.cols; j++) {
                h_img.at<cv::Vec3b>(i, j)[0] = h_img.at<cv::Vec3b>(i, j)[0] << 5;
                h_img.at<cv::Vec3b>(i, j)[1] = h_img.at<cv::Vec3b>(i, j)[1] << 5;
                h_img.at<cv::Vec3b>(i, j)[2] = h_img.at<cv::Vec3b>(i, j)[2] << 5;
            }
        }

        cv::imwrite("decoded_output.png", h_img);

    } else {
        cv::Mat h_img;
        h_img = cv::imread(h_img_path, cv::IMREAD_COLOR);
        cv::Mat plane_h_img(2*h_img.rows, 4*h_img.cols, CV_8UC3);

        for (uchar k = 0; k < 8; k++) {
            int dispx = k > 3 ? h_img.rows : 0;
            int dispy = k > 3 ? (k - 4)*h_img.cols : k*h_img.cols;

            for (int i = 0; i < h_img.rows; i++) {
                for (int j = 0; j < h_img.cols; j++) {
                    plane_h_img.at<cv::Vec3b>(i + dispx,j + dispy)[0] = h_img.at<cv::Vec3b>(i,j)[0] << k;
                    plane_h_img.at<cv::Vec3b>(i + dispx,j + dispy)[1] = h_img.at<cv::Vec3b>(i,j)[1] << k;
                    plane_h_img.at<cv::Vec3b>(i + dispx,j + dispy)[2] = h_img.at<cv::Vec3b>(i,j)[2] << k;
                }
            }
        }

        cv::imwrite("planes_output.png", plane_h_img);
    }

    return 0;
}