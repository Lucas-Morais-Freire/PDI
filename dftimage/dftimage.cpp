#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "cpplot/headers/cpplot.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void swapQuadrants(cv::Mat& image) {
    cv::Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para o maior
    // tamanho par possivel (-2 = 1111...1110)
    image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

    int centerX = image.cols / 2;
    int centerY = image.rows / 2;

    // rearranja os quadrantes da transformada de Fourier de forma que 
    // a origem fique no centro da imagem
    // A B   ->  D C
    // C D       B A
    A = image(cv::Rect(0, 0, centerX, centerY));
    B = image(cv::Rect(centerX, 0, centerX, centerY));
    C = image(cv::Rect(0, centerY, centerX, centerY));
    D = image(cv::Rect(centerX, centerY, centerX, centerY));

    // swap quadrants (Top-Left with Bottom-Right)
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // swap quadrant (Top-Right with Bottom-Left)
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

int main(int argc, char** argv) {
    cv::Mat image, padded, complexImage;
    std::vector<cv::Mat> planos; 

    // image = imread("../../senoide.png", cv::IMREAD_GRAYSCALE);
    // if (image.empty()) {
    //     std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
    //     return EXIT_FAILURE;
    // }

    cv::FileStorage fs("../../senoide.yml", cv::FileStorage::READ);

    if (!fs.isOpened()) {
        std::cerr << "Erro ao abrir o arquivo YAML." << std::endl;
        return -1;
    }

    fs["mat"] >> image;

    if (image.empty()) {
        std::cerr << "Erro ao ler a imagem advinda do arquivo." << std::endl;
        return -1;
    }

    // expande a imagem de entrada para o melhor tamanho no qual a DFT pode ser
    // executada, preenchendo com zeros a lateral inferior direita.
    int dft_M = cv::getOptimalDFTSize(image.rows);
    int dft_N = cv::getOptimalDFTSize(image.cols); 
    cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // prepara a matriz complexa para ser preenchida
    // primeiro a parte real, contendo a imagem de entrada
    planos.push_back(cv::Mat_<float>(padded)); 
    // depois a parte imaginaria com valores nulos
    planos.push_back(cv::Mat::zeros(padded.size(), CV_32F));

    // combina os planos em uma unica estrutura de dados complexa
    cv::merge(planos, complexImage);  

    // calcula a DFT
    cv::dft(complexImage, complexImage); 
    swapQuadrants(complexImage);

    // planos[0] : Re(DFT(image)
    // planos[1] : Im(DFT(image)
    cv::split(complexImage, planos);

    // calcula o espectro de magnitude e de fase (em radianos)
    cv::Mat magn, fase;
    cv::cartToPolar(planos[0], planos[1], magn, fase, false);
    cv::normalize(fase, fase, 0, 1, cv::NORM_MINMAX);

    // caso deseje apenas o espectro de magnitude da DFT, use:
    cv::magnitude(planos[0], planos[1], magn); 

    // some uma constante para evitar log(0)
    // log(1 + sqrt(Re(DFT(image))^2 + Im(DFT(image))^2))
    magn += cv::Scalar::all(1);

    // calcula o logaritmo da magnitude para exibir
    // com compressao de faixa dinamica
    log(magn, magn);
    
    cv::Mat magn_row = magn.row(magn.rows/2);
    std::vector<float> aux(magn_row.begin<float>(), magn_row.end<float>());
    std::vector<double> row_spec(aux.begin(), aux.end());

    double min_x = -1;
    double max_x = 1;
    Graph g(min_x,max_x,-0.5,*(std::max_element(row_spec.begin(), row_spec.end())) + 0.5);

    g.drawAxis(true, 0, false)
        .drawNumbers(false);

    std::vector<double> x_vals(row_spec.size());
    for (int i = 0; i < row_spec.size(); i++) {
        x_vals[i] = min_x + (max_x - min_x)/(row_spec.size() - 1)*i;
    }

    g.drawFunc(x_vals, row_spec, {0,0,255});

    g.write("./out.png");

    return EXIT_SUCCESS;
}