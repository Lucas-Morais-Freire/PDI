#include <iostream>
#include <opencv2/opencv.hpp>
#include <regex>
#include <string>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "quantidade de argumentos invalida.\n";
        return 0;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Abertura de imagem falhou.\n";
        return 0;
    }

    int p1[2], p2[2];
    std::string p1char, p2char;

    std::cout << "Insira dois pontos no formato (x,y) representando as arestas do retangulo a ser invertido.\nInsira o ponto 1: ";
    std::cin >> p1char;
    if (!std::regex_match(p1char, std::regex("\\([0-9]+,[0-9]+\\)"))) {
        std::cout << "entrada 1 invalida.\n";
        return 0;
    }
    p1[0] = stoi(p1char.substr(1, p1char.find(",")));
    p1[1] = stoi(p1char.substr(p1char.find(",") + 1, p1char.size() - p1char.find(",")));

    if (p1[0] < 0 || p1[0] >= image.rows || p1[1] < 0 || p1[1] >= image.cols) {
        std::cout << "os pontos especificados estao fora do escopo da imagem.\n";
        return 0;
    }

    std::cout << "Insira o ponto 2: ";
    std::cin >> p2char;
    if (!std::regex_match(p2char, std::regex("\\([0-9]+,[0-9]+\\)"))) {
        std::cout << "entrada 2 invalida.\n";
        return 0;
    }
    p2[0] = stoi(p2char.substr(1, p2char.find(",")));
    p2[1] = stoi(p2char.substr(p2char.find(",") + 1, p2char.size() - p2char.find(",")));

    if (p2[0] < 0 || p2[0] >= image.rows || p2[1] < 0 || p2[1] >= image.cols) {
        std::cout << "os pontos especificados estao fora do escopo da imagem.\n";
        return 0;
    }

    for (int i = p1[0] < p2[0] ? p1[0] : p2[0]; i < (p1[0] >= p2[0] ? p1[0] : p2[0]); i++) {
        for (int j = p1[1] < p2[1] ? p1[1] : p2[1]; j < (p1[1] >= p2[1] ? p1[1] : p2[1]); j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    cv::namedWindow("imagem processada", cv::WINDOW_NORMAL);
    cv::imshow("imagem processada", image);
    cv::waitKey();

    return 0;
}