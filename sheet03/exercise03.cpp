/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 10.05.2015
 * Blatt: 3
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 23.5.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cv.h>
#include <highgui.h>

/**
 * Aufgabe: Median-Filter (10 Punkte)
 *
 * Der Median-Filter ist ein einfacher nichtlinearer Filter, der sich gut eignet, um additives Bildrauschen zu entfernen.
 *
 * - Implementiere einen Median-Filter, ohne cvSmooth zu verwenden.
 */

/* TODO */
cv::Mat medianBlur(const cv::Mat &src, int k = 3, int channel = 0) {
	assert(src.channels() > channel);
	//float or int? we do not know the underlying conversion, so just demand float for now
	assert(src.depth() == CV_32F);
	
    cv::Mat dst = src.clone();
	std::vector<float> values;
	int dx = k/2;
	int dy = k/2;

	for(int y = dy; y < src.rows-dy; ++y) {
		for(int x = dx; x < src.cols-dx; ++x) {
			values.clear();

			for(int wy = -dy; wy <= dy; ++wy) {
				for(int wx = -dx; wx <= dx; ++wx) {
					values.push_back(*(&(src.at<float>(y+wy,x+wx))+channel));
				}
			}
			std::sort(values.begin(), values.end());
			*(&(dst.at<float>(y,x))+channel) = values[values.size() / 2];
		}
	}
    return dst;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
		exit(1);
	}

	// load the input image
	IplImage *img = cvLoadImage(argv[1]);


	CvSize size = cvGetSize(img);

	/**
	 * - Wende den Median-Filter auf ein Graustufenbild an.
	 */

/* TODO */
	IplImage *gray = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage *imageFloat = cvCreateImage(size, IPL_DEPTH_32F, img->nChannels);
	cvConvertScale(img, imageFloat, 1.0 / 255.0);
	cvCvtColor(imageFloat, gray, CV_BGR2GRAY);

	cvNamedWindow("Gray");
	cvShowImage("Gray", gray);
	cvWaitKey(0);

    cv::Mat img02(imageFloat);

    cv::Mat median = medianBlur(gray, 7);

	cvNamedWindow("Median");
    cv::imshow("Median", median);
	cvWaitKey(0);

	cv::Mat blue, green, red;
	// "channels" is a vector of 3 Mat arrays:
	std::vector<cv::Mat> channels_vector(3);
	// split img:
	cv::split(img02, channels_vector);
	// get the channels (BGR)
	blue = medianBlur(channels_vector[0], 7);
	green = medianBlur(channels_vector[1], 7);
	red = medianBlur(channels_vector[2], 7);
	
	cv::namedWindow( "Median Blue", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Blue", 100, 100);
	cv::imshow( "Median Blue", blue );                   
	cv::waitKey(0);
	cv::namedWindow( "Median Green", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Green", 100, 100);
	cv::imshow( "Median Green", green );                  
	cv::waitKey(0);
	cv::namedWindow( "Median Red", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Red", 100, 100);
	cv::imshow( "Median Red", red );                   
	cv::waitKey(0);


	/**
	 * - Wende den Median-Filter auf die einzelnen Kanäle eines Farbbilds an
	 */

/* TODO */


	/**
	 * - Wie kann man ungewollte Farbverschiebungen vermeiden?
	 */

	/**
	 * Aufgabe: Hough-Transformation (10 Punkte)
	 *
	 * Die Hough-Transformation kann für das Finden von Linien in Bildern verwendet werden.
	 *
	 * In dieser Aufgabe sollst du die Hough-Transformation implementieren.
	 * - Lade ein Testbild und wandle es in Graustufen um.
	 */

/* TODO */


	/**
	 * - Erzeuge ein Gradientenbild. Verwende dazu einen Filter deiner Wahl. Begründe die Wahl des Kantendetektors.
	 */

/* TODO */


	/**
	 * - Transformiere das Gradientenbild in den Hough-Raum und zeige das Bild an.
	 */
	IplImage *hough = cvCreateImage(cvSize(400, 400), IPL_DEPTH_32F, 1);

/* TODO */

	cvNamedWindow("Hough Space"); 
	cvShowImage("Hough Space", hough);
	cvWaitKey(0);
	
	/**
	 * - Finde die markantesten Linien und zeichne diese in das Originalbild ein.
	 */

/* TODO */

	cvNamedWindow("Hough Lines");
	cvShowImage("Hough Lines", img);
	cvWaitKey(0); 
}

