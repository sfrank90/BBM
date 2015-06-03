/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 23.05.2015
 * Blatt: 4
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 5.6.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cv.h>
#include <highgui.h>

template<typename T> const T &min(const T &a, const T &b) { return a < b ? a : b; }
template<typename T> const T &max(const T &a, const T &b) { return a > b ? a : b; }

int main(int argc, char *argv[]) {
	if (argc < 3){
		printf("Usage: %s <image-file-name1> <image-file-name2>\n", argv[0]);
		exit(1);
	}

	IplImage* img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (!img1) {
		printf("Could not load image file: %s\n", argv[1]);
		exit(1);
	}
	IplImage* img1f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, 1);
	cvConvertScale(img1, img1f, 1.0 / 255.0);

	IplImage* img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (!img2) {
		printf("Could not load image file: %s\n", argv[2]);
		exit(1);
	}
	IplImage* img2f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, 1);
	cvConvertScale(img2, img2f, 1.0 / 255.0);

	/**
	 * Aufgabe: Homographien (5 Punkte)
	 * 
	 * Unter der Annahme, dass Bilder mit einer verzerrungsfreien Lochbildkamera
	 * aufgenommen werden, kann man Aufnahmen mit verschiedenen Bildebenen und
	 * gleichem Projektionszentren durch projektive Abbildungen, sogenannte
	 * Homographien, beschreiben.
	 * 
	 * - Schreibe Translation als Homographie auf (auf Papier!).
	 * - Verschiebe die Bildebene eines Testbildes um 20 Pixel nach rechts, ohne
	 *   das Projektionszentrum zu ändern. Benutze dafür \code{cvWarpPerspective}.
	 * - Wieviele Punktkorrespondenzen benötigt man mindestens, um eine projektive
	 *   Abbildung zwischen zwei Bildern bis auf eine Skalierung eindeutig zu
	 *   bestimmen? Warum? (Schriftlich beantworten!)
	 */

/* TODO */
	IplImage* img_moved = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, 1);
	float data[] = { 1, 0, -20, 0, 1, 0, 0, 0, 1 };
	CvMat* trans = cvCreateMatHeader(3, 3, CV_32FC1);
	cvSetData(trans, data, trans->step);
	cvWarpPerspective(img1f, img_moved, trans);
	cv::namedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cv::Mat img_moved_final(img_moved);
	cv::imshow("mainWin", img_moved_final);
	cvWaitKey(0);

	/**
	 * Aufgabe: Panorama (15 Punkte)
	 *
	 * Ziel dieser Aufgabe ist es, aus zwei gegebenen Bildern ein Panorama zu konstruieren.
	 * \begin{center}
	 * \includegraphics[width = 0.3\linewidth]{left.png}
	 * \includegraphics[width = 0.3\linewidth]{right.png}
	 * \end{center}
	 * 
	 * Dafür muss zunächst aus den gegeben Punktkorrespondenzen
	 * \begin{center}
	 * \begin{tabular}{|c|c|}
	 * \hline
	 * linkes Bild & rechtes Bild \\
	 * $(x, y)$ & $(x, y)$ \\ \hline \hline
	 * (463, 164) & (225, 179)\\ \hline
	 * (530, 357) & (294, 370)\\ \hline
	 * (618, 357) &(379, 367)\\ \hline
	 * (610, 153) & (369, 168)\\ \hline
	 * \end{tabular}
	 * \end{center}
	 * eine perspektivische Transformation bestimmt werden, mit der die Bilder auf eine gemeinsame Bildebene transformiert werden können.
	 * 
	 * - Berechne die Transformation aus den gegebenen Punktkorrespondenzen.
	 *   Benutze die Funktion \code{cvGetPerspectiveTransform}. Was ist die
	 *   zentrale Idee des DLT-Algorithmus, wie er in der Vorlesung vorgestellt
	 *   wurde?
	*/

/* TODO */
	CvMat *P = cvCreateMat(3, 3, CV_32FC1);
	CvPoint points1[] = { cvPoint(463, 164), cvPoint(530, 357), cvPoint(618, 357), cvPoint(610, 153) };
	CvPoint points2[] = { cvPoint(225, 179), cvPoint(294, 370), cvPoint(379, 367), cvPoint(369, 168) };
	CvPoint2D32f pt1[4], pt2[4];
	for (int i = 0; i < 4; ++i) {
		pt2[i].x = points2[i].x;
		pt2[i].y = points2[i].y;
		pt1[i].x = points1[i].x;
		pt1[i].y = points1[i].y;
	}
	cvGetPerspectiveTransform(pt1, pt2, P);
	
	/**
	 * - Bestimme die notwendige Bildgröße für das Panoramabild.
	 */

/* TODO */
	int h = img1f->height - 1;
	int w = img1f->width - 1;
	float p1[] = { 0.0, 0.0, 1.0 };
	float p2[] = { 0.0, (float)(h), 1.0 };
	float p3[] = { (float)(w), (float)(h), 1.0 };
	float p4[] = { (float)(w), 0.0, 1.0 };

	
	cv::Mat P1 = P * cv::Mat(3, 1, CV_32FC1, p1);
	cv::Mat P2 = P * cv::Mat(3, 1, CV_32FC1, p2);
	cv::Mat P3 = P * cv::Mat(3, 1, CV_32FC1, p3);
	cv::Mat P4 = P * cv::Mat(3, 1, CV_32FC1, p4);

	// mustn't be zero
	assert(P1.at<float>(2,0) != 0 && P2.at<float>(2,0) != 0 && P3.at<float>(2,0) != 0 && P4.at<float>(2,0) != 0);

	P1 = P1 / P1.at<float>(2,0);
	P1 = P2 / P2.at<float>(2,0);
	P1 = P3 / P3.at<float>(2,0);
	P1 = P4 / P4.at<float>(2,0);



	/**
	 * - Projiziere das linke Bild in die Bildebene des rechten Bildes. Beachte
	 *   dabei, dass auch der linke Bildrand in das Panoramabild projiziert
	 *   wird.
	 */

/* TODO */

	std::vector<cv::Mat&> matrices;
	matrices.push_back(P1);
	matrices.push_back(P2);
	matrices.push_back(P3);
	matrices.push_back(P4);
	cv::Point minP(P1.at<float>(0,0), P1.at<float>(1,0)), maxP(P1.at<float>(0,0), P1.at<float>(1,0));
	for(int i = 0; i < matrices.size(); ++i) {
		for(int j = 0; j < 2; ++j) {
			minP.x = (int)(min(matrices[i].at<float>(0,0), (float)minP.x));
			minP.y = (int)(min(matrices[i].at<float>(1,0), (float)minP.y));

			maxP.x = (int)(max(matrices[i].at<float>(0,0), (float)maxP.x)+1.0);
			maxP.y = (int)(max(matrices[i].at<float>(1,0), (float)maxP.y)+1.0);
		}
	}

	// create image

	cv::Mat Panorama = cv::Mat(cv::Size(maxP-minP),  CV_32FC1);
	/**
	 * - Bilde das Panoramabild, so dass Pixel, für die zwei Werte vorhanden sind,
	 *   den Mittelwert zugeordnet bekommen.
	 */

/* TODO */

	/**
	 * - Zeige das Panoramabild an.
	 */

/* TODO */

}

