#include <iostream>
#include <opencv2/opencv.hpp>

int floodcount(cv::Mat& image, int i, int j, uchar label) {
    image.at<uchar>(i, j) = label;
    int count = 1;
    if (image.at<uchar>(i - 1, j) == 0) {
        count += floodcount(image, i - 1, j, label);
    }
    if (image.at<uchar>(i + 1, j) == 0) {
        count += floodcount(image, i + 1, j, label);
    }
    if (image.at<uchar>(i, j - 1) == 0) {
        count += floodcount(image, i, j - 1, label);
    }
    if (image.at<uchar>(i, j + 1) == 0) {
        count += floodcount(image, i, j + 1, label);
    }
    return count;
}

void floodtransfer(cv::Mat& labelled, cv::Mat& unlabelled, int i, int j, uchar label) {
    unlabelled.at<uchar>(i, j) = 0;
    labelled.at<uchar>(i, j) = 0;
    if (labelled.at<uchar>(i - 1, j) == label) {
        floodtransfer(labelled, unlabelled, i - 1, j, label);
    }
    if (labelled.at<uchar>(i + 1, j) == label) {
        floodtransfer(labelled, unlabelled, i + 1, j, label);
    }
    if (labelled.at<uchar>(i, j - 1) == label) {
        floodtransfer(labelled, unlabelled, i, j - 1, label);
    }
    if (labelled.at<uchar>(i, j + 1) == label) {
        floodtransfer(labelled, unlabelled, i, j + 1, label);
    }
}

int main(int argc, char** argv) {
    cv::Mat image;
    cv::Mat str;

    if (argc != 2) {
        std::cout << "morfologia entrada saida\n";
    }

    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    // image = cv::imread("../../digitos-1.png", cv::IMREAD_GRAYSCALE);

    if(image.empty()) {
        std::cout << "Erro ao carregar a imagem: " << argv[1] << std::endl;
        return -1;
    }

    // encontrar o ponto decimal via labelling. O ponto será aquele com menos pixels em sua área.
    
    cv::Mat aux;
    image.copyTo(aux);
    uchar labels = 0;
    int min = image.rows*image.cols;
    uchar min_label = 0;
    int min_j = 0; int min_i = 0;
    int count;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (aux.at<uchar>(i, j) == 0) {
                labels++;
                count = floodcount(aux, i, j, labels);
                if (count < min) {
                    min = count;
                    min_i = i;
                    min_j = j;
                    min_label = labels;
                }
            }
        }
    }

    cv::floodFill(image, cv::Point(min_j, min_i), 255);

    // testar todas as linhas da imagem para tentar descobrir qual é aquela que está no meio.

    std::vector<int> counts(image.rows);
    for (int i = 0; i < image.rows; i++) {
        counts[i] = 0;
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 0) {
                counts[i]++;
            }
        }
    }

    // calcular o valor medio:

    float total = 0;
    for (int i = 0; i < counts.size(); i++) {
        total += counts[i];
    }
    total /= counts.size();

    // procura quais indices representam transicoes sobre a media:
    
    std::vector<int> transitions;
    for (int i = 1; i < counts.size(); i++) {
        if (counts[i - 1] < total && counts[i] >= total || counts[i - 1] > total && counts[i] <= total) {
            transitions.push_back(i);
        }
    }

    // o primeiro e o ultimo elemento sao quase que garantidos de serem o inicio e o final dos digitos, entao escolherei
    // os dois elementos de transitions que estao mais proximos para serem os delimitadores da abertura central.

    int op_end = (transitions[0] + transitions[transitions.size() - 1])/2;
    int op_beg = op_end;
    for (int i = 0; i < transitions.size(); i++) {
        if (transitions[i] >= op_end) {
            op_end = transitions[i];
            op_beg = transitions[i - 1];
            break;
        }
    }

    str = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((op_end - op_beg)/2 + 2, op_end - op_beg + 2));
    cv::morphologyEx(image, image, cv::MORPH_OPEN, str, cv::Point(-1,-1), 1, 0, 255);

    floodtransfer(aux, image, min_i, min_j, min_label);
    
    cv::imwrite("exer12.png", image);

    return 0;
}