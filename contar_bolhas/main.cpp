#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // matriz que sera modificada para a contagem
    cv::Mat image;
    image = cv::imread("bolhas.png", cv::IMREAD_GRAYSCALE);

    if (image.data == NULL) {
        std::cout << "Falha na abertura da imagem.\n";
        exit(-1);
    }

    // esta matriz ira servir para vizualizar a contagem    
    cv::Mat canvas;
    image.copyTo(canvas);

    cv::Point p;

    // percorre a borda da imagem, efetivamente "apagando" quaisquer bolhas.
    for (int i = 0; i < image.rows; i++) {
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

    // com isso, eu faco a cor de fundo ser diferente da cor dos buracos.
    cv::floodFill(image, p, 254);

    // agora, vou começar a contar a quantidade de bolhas e de bolhas com buracos.
    uchar nbolhas = 0;
    
    // aqui irei guardar a quantidade de buracos em cada bolha.
    // como o 0 esta reservado para os buracos, 255 para as bolhas originais e 254 para o fundo, so teremos, no maximo, como contar 253 bolhas.
    std::vector<uchar> tem_bolha(253, 0);
    for (int i = 0; i < image.rows; i++) {

        // comeco da coluna 1 para poder testar dois pixels ao mesmo tempo.
        // nao interfere na contagem de bolhas pois ja sabemos que, na borda, nao ha bolhas.
        for (int j = 1; j < image.cols && nbolhas < 254; j++) {
            // sempre que encontrar uma bolha, executar um floodfill naquela posicao para demarca-la com a sua identificacao.
            if (image.at<uchar>(i, j) == 255) {
                p.x = j;
                p.y = i;
                nbolhas++;
                cv::floodFill(image, p, nbolhas);

                // tambem faco um floodfill com outra cor numa copia para vizualizarmos depois.
                cv::floodFill(canvas, p, 85);

            // se chegamos em um buraco que faz borda com uma bolha, aumentamos a quantidade de buracos para aquela bolha e executamos um floodfill.
            } else if (image.at<uchar>(i, j) == 0 && image.at<uchar>(i, j - 1) <= nbolhas) {
                p.x = j;
                p.y = i;
                tem_bolha[image.at<uchar>(i, j - 1) - 1]++;
                cv::floodFill(image, p, 254);

                // aqui é para fins de vizualizacao.
                cv::floodFill(canvas, p, 170);
            }
        }
    }

    // agora, contar quantos buracos temos no total, e quantas bolhas tem buracos.
    int nbolhas_com_buracos = 0;
    int nburacos = 0;
    for (std::vector<uchar>::iterator iter = tem_bolha.begin(); iter != tem_bolha.end(); iter++) {
        nburacos += (*iter);
        nbolhas_com_buracos += (*iter > 0) ? 1 : 0;
    }

    std::cout << "a imagem tem " << (int)nbolhas << " bolhas. " << nbolhas_com_buracos << " delas tem buracos, e temos " << nburacos << " buracos no total.\n";

    // mostrar a imagem de vizualizacao.
    cv::imwrite("contagem_bolhas.png", canvas);

    return 0;
}