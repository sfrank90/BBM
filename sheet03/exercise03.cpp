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

cv::Mat houghTransform(const cv::Mat &src, int thetaRes=512, int distRes=512) {
    cv::Mat accumBuffer(thetaRes, distRes, CV_32F);
    accumBuffer.setTo(0);

    int xres = src.cols, yres = src.rows;
    float maxDist = sqrt(pow(xres, 2) + pow(yres, 2));
    float distScale = distRes / maxDist;

    float maxValue = 0;
    for (int y = 0; y < yres; ++y) {
        for (int x = 0; x < xres; ++x) {
            if (src.at<uchar>(y, x) == 255) {
                for(int i = 0; i < thetaRes; ++i) {
                    float theta = 2 * i * CV_PI / thetaRes;
                    float dist = x * cos(theta) + y * sin(theta);
                    dist *= distScale;
                    if(dist > 0 && dist < maxDist) {
                        int cx = i, cy = dist;
                        accumBuffer.at<float>(cy, cx) += 1;
                        maxValue = std::max(accumBuffer.at<float>(cy, cx), maxValue);
                    }
                }
            }
        }
    }

    //normalize accumBuffer
    for (int y = 0; y < accumBuffer.rows; ++y) {
        for (int x = 0; x < accumBuffer.cols; ++x) {
            if(maxValue > 0)
                accumBuffer.at<float>(y, x) /= maxValue;
        }
    }
    return accumBuffer;
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

    int kernel_size = 3;
    cv::Mat median = medianBlur(gray, kernel_size);

	cvNamedWindow("Median");
    cv::imshow("Median", median);
	cvWaitKey(0);


	/**
	 * - Wende den Median-Filter auf die einzelnen Kanäle eines Farbbilds an
	 */

/* TODO */
	cv::Mat blue, green, red;
	// "channels" is a vector of 3 Mat arrays:
	std::vector<cv::Mat> channels_vector(3);
	// split img:
	cv::split(img02, channels_vector);
	// get the channels (BGR)
	blue = medianBlur(channels_vector[0], kernel_size);
	green = medianBlur(channels_vector[1], kernel_size);
	red = medianBlur(channels_vector[2], kernel_size);
	
	cv::namedWindow( "Median Red", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Red", 100, 100);
	cv::imshow( "Median Red", red );                   
	cv::waitKey(0);
	cv::namedWindow( "Median Green", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Green", 100, 100);
	cv::imshow( "Median Green", green );                  
	cv::waitKey(0);
	cv::namedWindow( "Median Blue", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Blue", 100, 100);
	cv::imshow( "Median Blue", blue );                   
	cv::waitKey(0);

    std::vector<cv::Mat> channels;
    channels.push_back(blue);
    channels.push_back(green);
    channels.push_back(red);
    cv::Mat medianImg;
    cv::merge(channels, medianImg);

	cv::namedWindow( "Median Combined", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Median Combined", 100, 100);
	cv::imshow( "Median Combined", medianImg);                   
	cv::waitKey(0);


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
    cv::Mat lines = cv::imread("lines.png");
    cv::Mat lines_gray;
    cv::cvtColor(lines, lines_gray, CV_BGR2GRAY, 1);
    lines_gray.convertTo(lines_gray, CV_32F, 1.0/255.0);
	cv::namedWindow( "lines gray", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "lines gray", 100, 100);
	cv::imshow( "lines gray", lines_gray);
	cv::waitKey(0);


	/**
	 * - Erzeuge ein Gradientenbild. Verwende dazu einen Filter deiner Wahl. Begründe die Wahl des Kantendetektors.
	 */

/* TODO */
    cv::Mat canny = medianBlur(lines_gray, 5);
    canny.convertTo(canny, CV_8U, 255);
    cv::Canny(canny, canny, 1, 10);

	cv::namedWindow( "Canny", cv::WINDOW_AUTOSIZE );
	cv::moveWindow( "Canny", 100, 100);
	cv::imshow( "Canny", canny);
	cv::waitKey(0);

	/**
	 * - Transformiere das Gradientenbild in den Hough-Raum und zeige das Bild an.
	 */
    cv::Mat hough = houghTransform(canny);
	//IplImage *hough = cvCreateImage(cvSize(400, 400), IPL_DEPTH_32F, 1);

/* TODO */

	//cvNamedWindow("Hough Space"); 
	//cvShowImage("Hough Space", hough);
	//cvWaitKey(0);
	
	cv::namedWindow("Hough Space", cv::WINDOW_AUTOSIZE);
	cv::moveWindow( "Hough Space", 100, 100);
	cv::imshow( "Hough Space", hough);
	cv::waitKey(0);
	/**
	 * - Finde die markantesten Linien und zeichne diese in das Originalbild ein.
	 */

/* TODO */

	cvNamedWindow("Hough Lines");
	cvShowImage("Hough Lines", img);
	cvWaitKey(0); 
}

