#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
    cv::Mat image;
    cv::Mat imageBRG;
    int width, height;
    cv::VideoCapture cap;
    std::vector<int> hist(256, 0);
    int key;

    cap.open(0);

    if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    // a fim de mostrar a comparacao, duas janelas diferentes serao criadas.
    cv::namedWindow("equalizada");
    cv::namedWindow("normal");

    // este sera o vetor da funcao distribuicao acumulada
    std::vector<int> cdf(256, 0);

    while(1){
        // capturar a imagem colorida
        cap >> imageBRG;

        // converter a imagem colorida para escala de cinza
        cv::cvtColor(imageBRG, image, cv::COLOR_BGR2GRAY);

        // mostrar a imagem colorida
        cv::imshow("normal", image);

        // zerar o histograma
        for (int i = 0; i <= 255; i++) {
            hist[i] = 0;
        }
        
        // calcular o histograma
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                hist[(int)image.at<uchar>(i,j)]++;
            }
        }

        // calcular a cdf
        cdf[0] = hist[0];
        
        for (int i = 1; i <= 255; i++) {
            cdf[i] = hist[i] + cdf[i - 1];
        }

        // normalizamos a cdf para obter a tabela:
        for (int i = 0; i <= 255; i++) {
            cdf[i] = (cdf[i]*255)/cdf[255];
        }

        // aplicamos a tabela e salvamos na propria imagem
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                image.at<uchar>(i, j) = (uchar)cdf[(int)image.at<uchar>(i, j)];
            }
        }

        cv::imshow("equalizada", image);
        key = cv::waitKey(30);
        if(key == 27) break;
        l++;
    }

    return 0;
}