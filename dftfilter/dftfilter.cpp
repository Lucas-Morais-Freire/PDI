#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

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

void makeFilter(const cv::Mat &image, cv::Mat &filter, float gL, float gH, float c, float D0){
    cv::Mat_<float> filter2D(image.rows, image.cols);
    int centerX = image.cols / 2;
    int centerY = image.rows / 2;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            filter2D.at<float>(i, j) = (gH - gL)*(1 - exp(-c*(((i - centerY)*(i - centerY) + (j - centerX)*(j - centerX))/(D0*D0)))) + gL;
        }
    }

    cv::Mat planes[] = {cv::Mat_<float>(filter2D), cv::Mat::zeros(filter2D.size(), CV_32F)};
    cv::merge(planes, 2, filter);
}

cv::Mat filter, complexImage, complexImageFiltered, image, result;
std::vector<cv::Mat> planos;
int gL = 1, gH = 2, c = 1, D0 = 4;

void applyFilter(int, void*) {
    // cria e aplica o filtro
    makeFilter(complexImage, filter, gL/10.0, gH/10.0, c/10.0, D0/10.0);
    cv::mulSpectrums(complexImage, filter, complexImageFiltered, 0);

    // calcula a DFT inversa
    swapQuadrants(complexImageFiltered);
    cv::idft(complexImageFiltered, complexImageFiltered);

    // planos[0] : Re(DFT(image)
    // planos[1] : Im(DFT(image)
    cv::split(complexImageFiltered, planos);

    // recorta a imagem filtrada para o tamanho original
    // selecionando a regiao de interesse (roi)
    cv::Rect roi(0, 0, image.cols, image.rows);
    result = planos[0](roi);

    // normaliza a parte real para exibicao
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);

    cv::imshow("saida", result);
}

int main(int argc, char** argv) {
    cv::Mat padded;
    std::vector<cv::Mat> planos; 

    // abre a imagem
    image = imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
        return EXIT_FAILURE;
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

    // inverte os quadrantes
    swapQuadrants(complexImage);
    
    // cria uma janela para mostrar a saida
    cv::namedWindow("saida", cv::WINDOW_NORMAL);

    cv::createTrackbar("gL", "saida", &gL, 100, applyFilter, nullptr);
    cv::createTrackbar("gH", "saida", &gH, 100, applyFilter, nullptr);
    cv::createTrackbar("c", "saida", &c, 100, applyFilter, nullptr);
    cv::createTrackbar("D0", "saida", &D0, 100, applyFilter, nullptr);
    cv::waitKey();

    cv::imwrite("dft-filter.png", result * 255);
    return EXIT_SUCCESS;
}