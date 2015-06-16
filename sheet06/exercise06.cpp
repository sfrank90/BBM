/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 12.06.2015
 * Blatt: 6
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 19.06.2012.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <ctime>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace std;

void createDepthMap(const cv::Mat &src1, const cv::Mat &src2, cv::Mat &dst, int radius_from = -10, int radius_to = 10) {

	cv::Mat src1_clone = src1.clone();
	src1_clone.convertTo(src1_clone, CV_32FC3);
	cv::Mat src2_clone = src2.clone();
	src2_clone.convertTo(src2_clone, CV_32FC3);

	dst = cv::Mat(src1.rows, src1.cols, CV_32F);

	float min_value = numeric_limits<float>::infinity();
	float max_value = -numeric_limits<float>::infinity();;
	for(int x = 0; x < src1.cols; ++x) {
		for(int y = 0; y < src1.rows; ++y) {
			const cv::Vec3f &v1 = src1_clone.at<cv::Vec3f>(y,x); 

			float best_radius = 0;
			float min_error = numeric_limits<float>::infinity();
			// => rektifiziert, horizontal reicht?
			for(int rx = radius_from; rx <= radius_to; ++rx) {
				if((x+rx) < 0 || (x+rx) >= src2.cols)
					continue;
				const cv::Vec3f &v2 = src2_clone.at<cv::Vec3f>(y,x+rx); 

				//rgb-disparitaet
				float d = abs(v1[0]-v2[0]) + abs(v1[1]-v2[1]) + abs(v1[2]-v2[2]);

				if(d < min_error) {
					best_radius = rx;
					min_error = d;
				}
			}
			if(best_radius < min_value) min_value = best_radius;
			if(best_radius > max_value) max_value = best_radius;
			dst.at<float>(y,x) = best_radius;
		}
	}
	///scale map
	for(int x = 0; x < dst.cols; ++x) {
		for(int y = 0; y < dst.rows; ++y) {
			//dst.at<float>(y,x) /= max_dist; 
			dst.at<float>(y,x) = 1.0f - (dst.at<float>(y,x) - min_value) / (max_value - min_value);
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc<3){
		cout << "Usage: bbm6 <image-file-name1> <image-file-name2>" << endl;
		exit(0);
	}

	// create a window
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 100, 100);

	// load an image
	IplImage *img1 = cvLoadImage(argv[1]);
	IplImage *img2 = cvLoadImage(argv[2]);

	if(!img1 || !img2) {
		cout << "Could not load image file: " << argv[1] << " or " << argv[2] << endl;
		exit(1);
	}

	//Convert to suitable format
	IplImage* img1f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, img1->nChannels);
	IplImage* img2f = cvCreateImage(cvGetSize(img2), IPL_DEPTH_32F, img2->nChannels);

	cvConvertScale(img1, img1f, 1.0/255.0);
	cvConvertScale(img2, img2f, 1.0/255.0);

	//Create depth map
	//IplImage* depth = cvCreateImage(cvGetSize(img2), IPL_DEPTH_32F, 1);
	cv::Mat depth;
	/**
	 * Aufgabe: Erzeugen einer Tiefenkarte (10 Punkte)
	 *
	 * Es soll für ein Stereo-Bildpaar eine Tiefenkarte erzeugt werden.  Die beiden
	 * Bilder können als bereits rektifiziert angenommen werden.  Es ist also eine
	 * Disparität für jeden Pixel zu schätzen.  Der Suchradius soll dabei
	 * vorgegeben werden können.  Der Pixel in Bild 1 an der Position $\vec{x}_1$
	 * ist beschrieben durch einen Deskriptor $d_1(\vec{x}_1)$.  Für jeden Pixel
	 * $\vec{x}_2$ innerhalb des Suchradius muss nun der Deskriptor
	 * $d_2(\vec{x}_2)$ mit $d_1$ verglichen werden.  Verwende als Deskriptor
	 * zunächst einfach die Farbe des Pixels.  Zeige die erzeugte Tiefenkarte in
	 * einem Fenster an.
	 */

/* TODO */
    createDepthMap(img1f,img2f,depth,-10,10);
    cv::imshow("mainWin",depth);
	cv::waitKey(0);

	/**
	 * Aufgabe: Robustere Methoden (20 Punkte)
	 *
	 * In dieser Aufgabe soll die Tiefenschätzung robuster gegenüber Fehlern
	 * gemacht werden.  Hier ist Eure Kreativität gefragt.  Überlegt Euch wie die
	 * Disparität zuverlässiger bestimmt werden kann und implementiert Euren
	 * Ansatz.  Möglich wären zum Beispiel:
	 * - bessere Deskriptoren, etwa ein Fenster von mehreren Pixeln Breite
	 * - Regularisierung, d.h. benachbarte Pixel sollten ähnliche Tiefenwerte
	 *   haben, auch wenn dadurch die Deskriptor etwas weniger gut passen; dazu
	 *   könnte man beispielsweise mit der Lösung der ersten Aufgabe beginnen und
	 *   in einem zweiten Schritt die Disparitäten der Nachbarpixel in den
	 *   Deskriptor mit einbauen. Das Ganze würde man dann solange wiederholen, bis
	 *   sich nichts mehr ändert.
	 */

/* TODO */
	//createBetterDepthMapByBetterDescriptor(img1f,img2f,depth,radius);
    //cv::imshow("better solution",depth);
	//cv::waitKey(0);

	return 0;
}

