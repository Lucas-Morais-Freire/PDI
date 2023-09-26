#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <cmath>
#include "cpplot/headers/cpplot.hpp"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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
void drawLine(double xa, double ya, double xb, double yb, cv::Mat m, double sw, cv::Vec3b c) {
	// determinar a ordem dos pontos no plano
	// P1 = (xa, ya) e P2 = (xb, yb)
	double xmin = xa < xb ? xa : xb;
	double xmax = xa >= xb ? xa : xb;
	double ymin = ya < yb ? ya : yb;
	double ymax = ya >= yb ? ya : yb;
	
	// determinar os limites da regiao retangular de teste
	int x1 = xmin - sw < 0 ? 0 : (xmin - sw >= m.rows ? m.rows - 1 : (int)(xmin - sw));
	int y1 = ymin - sw < 0 ? 0 : (ymin - sw >= m.cols ? m.cols - 1 : (int)(ymin - sw));
	int x2 = xmax + sw >= m.rows ? m.rows - 1 : (xmax + sw < 0 ? 0 : (int)(xmax + sw) + 1);
	int y2 = ymax + sw >= m.cols ? m.cols - 1 : (ymax + sw < 0 ? 0 : (int)(ymax + sw) + 1);

	double d;
	double pp1;
	double pp2;
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			// primeiro, testar se o ponto pode ser projetado perpendicularmente ao segmento de reta.
			// checar se o produto escalar (P2 - P1)*(P - P1) >= 0 e (P1 - P2)*(P - P2) >= 0.
			pp1 = (xb - xa)*(i - xa) + (yb - ya)*(j - ya);
			pp2 = (xa - xb)*(i - xb) + (ya - yb)*(j - yb);
			if (pp1 >= 0 && pp2 >= 0) {
				// se sim, calcular a distancia deste ponto ate a reta. baseado nessa distancia, colorir o pixel com esta cor.
				// d = ||(P2 - P1) x (P - P1)||/||(P2 - P1)||.
				d = abs((xb - xa)*(j - ya) - (i - xa)*(yb - ya))/sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
				if (d < sw) {
					// se o ponto estiver dentro da distancia especificada, pintar da cor escolhida.
					m.at<cv::Vec3b>(i, j) = c;
				} else if (d < sw + 1) {
					// se estiver a ate 1 pixel de distancia, interpolar com a cor de fundo.
					m.at<cv::Vec3b>(i, j)[0] = (uchar)((m.at<cv::Vec3b>(i, j)[0] - c[0])*d + c[0] - (m.at<cv::Vec3b>(i, j)[0] - c[0])*sw);
					m.at<cv::Vec3b>(i, j)[1] = (uchar)((m.at<cv::Vec3b>(i, j)[1] - c[1])*d + c[1] - (m.at<cv::Vec3b>(i, j)[1] - c[1])*sw);
					m.at<cv::Vec3b>(i, j)[2] = (uchar)((m.at<cv::Vec3b>(i, j)[2] - c[2])*d + c[2] - (m.at<cv::Vec3b>(i, j)[2] - c[2])*sw);
				}
			} else {
				// se o ponto nao puder ser projetado, testar se fará parte da ponta arredondada da linha.
				if (pp1 < 0) { // se o produto escalar (P2 - P1)*(P - P1) < 0, entao o ponto esta mais proximo de P1.
					d = sqrt((xa - i)*(xa - i) + (ya - j)*(ya - j));
				} else { // se nao, esta mais proximo de P2.
					d = sqrt((xb - i)*(xb - i) + (yb - j)*(yb - j));
				}
				if (d < sw) { // mesma ideia do caso anterior.
					m.at<cv::Vec3b>(i, j) = c;
				} else if (d < sw + 1) {
					m.at<cv::Vec3b>(i, j)[0] = (uchar)((m.at<cv::Vec3b>(i, j)[0] - c[0])*d + c[0] - (m.at<cv::Vec3b>(i, j)[0] - c[0])*sw);
					m.at<cv::Vec3b>(i, j)[1] = (uchar)((m.at<cv::Vec3b>(i, j)[1] - c[1])*d + c[1] - (m.at<cv::Vec3b>(i, j)[1] - c[1])*sw);
					m.at<cv::Vec3b>(i, j)[2] = (uchar)((m.at<cv::Vec3b>(i, j)[2] - c[2])*d + c[2] - (m.at<cv::Vec3b>(i, j)[2] - c[2])*sw);
				}
			}
		}
	}
}

void drawArrow(double xa, double ya, double xb, double yb, cv::Mat m, double sw, cv::Vec3b c, double asize, double angle) {
	drawLine(xa, ya, xb, yb, m, sw, c);

	angle = M_PI/180*angle;

	double S = sin(angle); double C = cos(angle);
	
	asize = asize/sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));

	drawLine(xb, yb, xb + asize*((xa - xb)*C + (yb - ya)*S), yb + asize*((ya - yb)*C + (xa - xb)*S), m, sw, c);
	drawLine(xb, yb, xb + asize*((xa - xb)*C + (ya - yb)*S), yb + asize*((ya - yb)*C + (xb - xa)*S), m, sw, c);
}

double iIdx(double y, int mr, double ymax, double ymin) {
	return mr - 1 - (mr - 1)/(ymax - ymin)*(y - ymin);
}
double jIdx(double x, int mc, double xmax, double xmin) {
	return (mc - 1)/(xmax - xmin)*(x - xmin);
}

double xpos(double j, int mc, double xmax, double xmin) {
	return (xmax - xmin)/(mc - 1)*j + xmin;
}
double ypos(double i, int mr, double ymax, double ymin) {
	return -(ymax - ymin)/(mr - 1)*(i - mr + 1) + ymin;
}

void drawCanvas(double xmin, double ymin, double xmax, double ymax, double xstep, double ystep, cv::Mat m, double sw, cv::Vec3b caxis, cv::Vec3b cbg, double asize, double angle) {
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			m.at<cv::Vec3b>(i, j) = cbg;
		}
	}

	drawArrow(iIdx(0, m.rows, ymax, ymin), -sw, iIdx(0, m.rows, ymax, ymin), m.cols - 1 - sw, m, sw, caxis, asize, angle);
	drawArrow(m.rows - 1 - sw, jIdx(0, m.cols, xmax, xmin), sw, jIdx(0, m.cols, xmax, xmin), m, sw, caxis, asize, angle);

	double ang = angle*M_PI/180;
	double tksize = asize*sin(ang);

	for (double x = xstep; x <= xpos(m.cols - 4 - asize*cos(ang) - 3*sw, m.cols, xmax, xmin); x += xstep) {
		drawLine(iIdx(0, m.rows, ymax, ymin) - tksize, jIdx(x, m.cols, xmax, xmin), iIdx(0, m.rows, ymax, ymin) + tksize, jIdx(x, m.cols, xmax, xmin), m, sw, caxis);
	}
	for (double x = -xstep; x >= xpos(sw + 3, m.cols, xmax, xmin); x -= xstep) {
		drawLine(iIdx(0, m.rows, ymax, ymin) - tksize, jIdx(x, m.cols, xmax, xmin), iIdx(0, m.rows, ymax, ymin) + tksize, jIdx(x, m.cols, xmax, xmin), m, sw, caxis);
	}
	for (double y = ystep; y <= ypos(asize*cos(ang) + 3*sw + 3, m.rows, ymax, ymin); y += ystep) {
		drawLine(iIdx(y, m.rows, ymax, ymin), jIdx(0, m.cols, xmax, xmin) - tksize, iIdx(y, m.rows, ymax, ymin), jIdx(0, m.cols, xmax, xmin) + tksize, m, sw, caxis);
	}
	for (double y = -ystep; y >= ypos(m.rows - 4 - sw, m.rows, ymax, ymin); y -= ystep) {
		drawLine(iIdx(y, m.rows, ymax, ymin), jIdx(0, m.cols, xmax, xmin) - tksize, iIdx(y, m.rows, ymax, ymin), jIdx(0, m.cols, xmax, xmin) + tksize, m, sw, caxis);
	}

}

void drawFunction(double (*func)(double), double xmin, double xmax, double ymin, double ymax, cv::Mat m, double sw, cv::Vec3b c) {
	cv::Mat posession(m.rows, m.cols, CV_8U);
	posession.setTo(0);
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

	Graph g(0, 2*M_PI, -2, 2, 1000, 500);

	g.drawFunc(sin, 3, {0,0,255});

	g.write("../../output1.png");
	g.setRange(g.xmin(), 20, g.ymin(), g.ymax());
	g.write("../../output2.png");
	g.setRes(1000, 250);
	g.write("../../output3.png");

	return 0;
}