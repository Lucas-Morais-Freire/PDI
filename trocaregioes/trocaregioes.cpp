#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "quantidade de argumentos invalida.";
        return 0;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (image.empty()) {
        std::cout << "Abertura de imagem falhou.";
        return 0;
    } else if (image.rows % 2 != 0 || image.cols % 2 != 0) {
        std::cout << "A imagem deve possuir numero par de pixels em suas dimensoes.";
        return 0;
    }

    uchar temp;
    int half_height = image.rows >> 1;
    int half_width = image.cols >> 1;
    
    for (int i = 0; i < half_height; i++) {
        for (int j = 0; j < half_width; j++) {
            temp = image.at<uchar>(i, j);
            image.at<uchar>(i, j) = image.at<uchar>(i + half_height, j + half_width);
            image.at<uchar>(i + half_height, j + half_width) = temp;
        }
    }

    for (int i = half_height; i < half_height << 1; i++) {
        for (int j = 0; j < half_width; j++) {
            temp = image.at<uchar>(i, j);
            image.at<uchar>(i, j) = image.at<uchar>(i - half_height, j + half_width);
            image.at<uchar>(i - half_height, j + half_width) = temp;
        }
    }

    cv::namedWindow("imagem invertida", cv::WINDOW_NORMAL);
    cv::imshow("imagem invertida", image);
    cv::waitKey();

    return 0;
}