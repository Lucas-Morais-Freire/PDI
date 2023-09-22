#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

#define M_PI 3.14159265358979323846

int SIDE = 256;
int PERIODOS = 4;

/**
 * @brief desenha uma semirreta entre dois pontos.
 * 
 * @param x1 coordenada x do primeiro ponto.
 * @param y1 coordenada y do primeiro ponto.
 * @param x2 coordenada x do segundo ponto.
 * @param y2 coordenada y do segundo ponto.
 * @param m  matriz cv::Mat do OpenCV do tipo CV_8UC3.
 * @param sw metade da largura da semirreta.
 * @param c  cor da reta.
 */
void drawLine(int x1, int y1, int x2, int y2, cv::Mat* m, double sw, cv::Vec3b* c) {
	// determinar a ordem dos pontos no plano
	// P1 = (xmin, ymin) e P2 = (xmax, ymax)
	int xmin = x1 < x2 ? x1 : x2;
	int xmax = x1 >= x2 ? x1 : x2;
	int ymin = y1 < y2 ? y1 : y2;
	int ymax = y1 >= y2 ? y1 : y2;
	
	// determinar os limites da regiao retangular de teste
	x1 = xmin - (int)sw - 1 < 0 ? 0 : xmin - (int)sw - 1;
	y1 = ymin - (int)sw - 1 < 0 ? 0 : ymin - (int)sw - 1;
	x2 = xmax + (int)sw + 1 >= m->rows ? m->rows - 1 : xmax + (int)sw + 1;
	y2 = ymax + (int)sw + 1 >= m->cols ? m->cols - 1 : ymax + (int)sw + 1;

	double d;
	int pp1;
	int pp2;
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			// primeiro, testar se o ponto pode ser projetado perpendicularmente ao segmento de reta.
			// checar se o produto escalar (P2 - P1)*(P - P1) >= 0 e (P1 - P2)*(P - P2) >= 0.
			pp1 = (xmax - xmin)*(i - xmin) + (ymax - ymin)*(j - ymin);
			pp2 = (xmin - xmax)*(i - xmax) + (ymin - ymax)*(j - ymax);
			if (pp1 >= 0 && pp2 >= 0) {
				// se sim, calcular a distancia deste ponto ate a reta. baseado nessa distancia, colorir o pixel com esta cor.
				// d = ||(P2 - P1) x (P - P1)||/||(P2 - P1)||.
				d = abs((xmax - xmin)*(j - ymin) - (i - xmin)*(ymax - ymin))/sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin));
				if (d < sw) {
					// se o ponto estiver dentro da distancia especificada, pintar da cor escolhida.
					m->at<cv::Vec3b>(i, j) = *c;
				} else if (d < sw + 1) {
					// se estiver a ate 1 pixel de distancia, interpolar com a cor de fundo.
					m->at<cv::Vec3b>(i, j)[0] = (uchar)((m->at<cv::Vec3b>(i, j)[0] - (*c)[0])*d + (*c)[0] - (m->at<cv::Vec3b>(i, j)[0] - (*c)[0])*sw);
					m->at<cv::Vec3b>(i, j)[1] = (uchar)((m->at<cv::Vec3b>(i, j)[1] - (*c)[1])*d + (*c)[1] - (m->at<cv::Vec3b>(i, j)[1] - (*c)[1])*sw);
					m->at<cv::Vec3b>(i, j)[2] = (uchar)((m->at<cv::Vec3b>(i, j)[2] - (*c)[2])*d + (*c)[2] - (m->at<cv::Vec3b>(i, j)[2] - (*c)[2])*sw);
				}
			} else {
				// se o ponto nao puder ser projetado, testar se fará parte da ponta arredondada da linha.
				if (pp1 < 0) { // se o produto escalar (P2 - P1)*(P - P1) < 0, entao o ponto esta mais proximo de P1.
					d = sqrt((xmin - i)*(xmin - i) + (ymin - j)*(ymin - j));
				} else { // se nao, esta mais proximo de P2.
					d = sqrt((xmax - i)*(xmax - i) + (ymax - j)*(ymax - j));
				}
				if (d < sw) { // mesma ideia do caso anterior.
					m->at<cv::Vec3b>(i, j) = *c;
				} else if (d < sw + 1) {
					m->at<cv::Vec3b>(i, j)[0] = (uchar)((m->at<cv::Vec3b>(i, j)[0] - (*c)[0])*d + (*c)[0] - (m->at<cv::Vec3b>(i, j)[0] - (*c)[0])*sw);
					m->at<cv::Vec3b>(i, j)[1] = (uchar)((m->at<cv::Vec3b>(i, j)[1] - (*c)[1])*d + (*c)[1] - (m->at<cv::Vec3b>(i, j)[1] - (*c)[1])*sw);
					m->at<cv::Vec3b>(i, j)[2] = (uchar)((m->at<cv::Vec3b>(i, j)[2] - (*c)[2])*d + (*c)[2] - (m->at<cv::Vec3b>(i, j)[2] - (*c)[2])*sw);
				}
			}
		}
	}
}

int main(int argc, char** argv) {
	// std::stringstream ss_img, ss_yml;
	// cv::Mat image;

	// ss_yml << "senoide-" << SIDE << ".yml";
	// image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

	// cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

	// for (int i = 0; i < SIDE; i++) {
	// for (int j = 0; j < SIDE; j++) {
	// 	image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
	// }
	// }

	// fs << "mat" << image;
	// fs.release();

	// cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
	// image.convertTo(image, CV_8U);
	// ss_img << "senoide-" << SIDE << ".png";
	// cv::imwrite(ss_img.str(), image);

	// fs.open(ss_yml.str(), cv::FileStorage::READ);
	// fs["mat"] >> image;

	// return 0;

	cv::Mat mat(300, 1000, CV_8UC3);
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			mat.at<cv::Vec3b>(i, j) = {255, 255, 255};
		}
	}

	cv::Vec3b color = {0, 0, 255};

	drawLine(100, 100, 1000, 700, &mat, 5	, &color);

	cv::imwrite("../../output.png", mat);

	return 0;
}