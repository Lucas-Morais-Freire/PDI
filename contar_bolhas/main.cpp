#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat image;
    image = cv::imread("../../bolhas.png", cv::IMREAD_GRAYSCALE); // matrix that will be modified for analysis purposes.

    cv::Mat canvas;
    image.copyTo(canvas); // matrix that will be modified to vizualize analysis result.

    cv::Point p;

    for (int i = 0; i < image.rows; i++) { // ignore bubbles that touch the edge of the image.
        int j = 0;
        while (j < image.cols) {
            if (image.at<uchar>(i, j) == 255) {
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 0);
            }
            
            if (i == 0 || i == image.rows - 1) {
                j++;
            } else {
                j += image.cols - 1;
            }
        }
    }

    p.x = 0;
    p.y = 0;

    cv::floodFill(image, p, 1); // make background color different than hole color.

    int nbubs = 0;
    int nholes = 0;
    for (int i = 0; i < image.rows; i++) { // count the number of bubbles and holes.
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 1);
                cv::floodFill(canvas, p, 85);
                nbubs++;
            } else if (image.at<uchar>(i, j) == 0) {
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 1);
                cv::floodFill(canvas, p, 170);
                nholes++;
            }
        }
    }

    std::cout << "image has " << nbubs << " bubbles and " << nholes << " of them have holes.\n";
    cv::imwrite("../../modified.png", canvas);

    return 0;
}