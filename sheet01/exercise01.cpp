/**
 * Vorlesung: Bildbasierte Modellierung SS 2012
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 17.04.2012
 * Blatt: 1
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 24.04.2012.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cv.h>
#include <highgui.h>

template<typename T> const T &min(const T &a, const T &b) { return a < b ? a : b; }
template<typename T> const T &max(const T &a, const T &b) { return a > b ? a : b; }

/**
 * Ziel der ersten Übung ist das Vertrautwerden mit der OpenCV-Bibliothek. 
 * Diese stellt nicht nur die Grundlagen zum Arbeiten mit Bildern zur Verfügung, 
 * sondern auch viele weitere in der Bilderverarbeitung häufig verwendete Funktionen.
 * Erhältlich ist sie als Open-Source-Projekt unter:
 * \begin{center}\url{http://opencv.orb/}\end{center}
 * Dokumentation findet sich beispielsweise im Buch \emph{Learning OpenCV} von Gary Bradski und Adrian Kaehler oder unter:
 * \begin{center}\url{http://http://docs.opencv.org/modules/refman.html}\end{center}
 */

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("Usage: main <image-file-name> <output-file-name> <kappa1> <kappa2>\n\7");
		exit(1);
	}

	/**
	 * Aufgabe: OpenCV starten (10 Punkte)
	 * 
	 * Erweitere die gegebene Programmgrundstruktur so, dass
	 * - ein Bild geladen werden kann.
	 */
	cv::Mat image;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

/* TODO */


	/**
	 * - die Höhe, Breite, Anzahl der Farbkanäle dieses Bildes ausgegeben wird.
	 */
	int width = image.cols;
	int height = image.rows;
	int channels = image.channels();;
	std::cout << "Image Attributes - width: " << width << ", height: " << height 
			<< ", number of channels: " << channels << std::endl;

/* TODO */


	/**
	 * - dieses Bild in einem \code{cvNamedWindow} angezeigt wird, bis eine Tastatureingabe erfolgt.
	 */
	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Display window", image );                   // show original image

    cv::waitKey(0);

/* TODO */


	/**
	 * - die drei Farbkanäle des Bildes nebeneinander angezeigt werden.
	 */
	cv::Mat blue, green, red;
	// "channels" is a vector of 3 Mat arrays:
	std::vector<cv::Mat> channels_vector(3);
	// split img:
	cv::split(image, channels_vector);
	// get the channels (BGR)
	blue = channels_vector[0];
	green = channels_vector[1];
	red = channels_vector[2];
	
	cv::namedWindow( "Blue Channel", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Blue Channel", blue );                   
	cv::namedWindow( "Green Channel", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Green Channel", green );                  
	cv::namedWindow( "Red Channel", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Red Channel", red );                   

/* TODO */


	/**
	 * - das Bild zusammen mit einem roten $10 \times 10$ Rechteck um die Bildmitte angezeigt wird.
	 */
	 
	cv::Point center = cv::Point( width / 2, height / 2 );
	cv::rectangle( image, 
               cvPoint(center.x-5,center.y-5),
               cvPoint(center.x+5,center.y+5),
               CV_RGB(255,0,0), 1, 8
             );
	std::cout << center.x << " " << center.y << std::endl;
	cv::namedWindow( "Rectangle Window", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Rectangle Window", image );  
	cv::waitKey(0);

/* TODO */


	/**
	 * Aufgabe: Bilder entzerren (10 Punkte)
	 *
	 * Das Bild \code{distorted.png}, Abb~\ref{fig:distorted}, wurde mit einer
	 * Weitwinkelkamera aufgenommen und zeigt starke radiale Verzerrung.  Aus der
	 * Vorlesung bekannt ist, dass die radiale Verzerrung oft durch
	 * $$ x = x_c + L(r) (x_d-x_c) \quad y = y_c + L(r) (y_d-y_c) $$
	 * ausgedrückt wird, wo $(x, y)$ die idealen Koordinaten sind, $({x_d}, {y_d})$
	 * die verzerrten Koordinaten und $L({r})$ eine Funktion, die nur von der
	 * Entfernung ${r}=\sqrt{(x-x_c)^2 + ({y}-y_c)^2}$ zum Verzerrungszentrum
	 * $(x_c,y_c)$ abhängt.
	 * Die Funktion $L(r)$ kann durch ihre Taylorentwicklung $L(r) = 1+ \kappa_1 r
	 * + \kappa_2 r^2 + \kappa_3 r^3 + \cdots$ beschrieben werden.
	 * Verschiedene Möglichkeiten, die Parameter zu bestimmen, sind denkbar und
	 * werden beispielsweise in \emph{Multiple View Geometry} von Hartley und
	 * Zisserman beschrieben, sollen hier aber nicht zur Anwendung kommen.
	 *
	 * Erweitere die gegebene Programmgrundstruktur so, dass 
	 * - die Funktion $L$ mit Taylorentwicklung 2. Ordnung approximiert wird,
	 *   wobei das Verzerrungszentrum der Bildmitte entspricht.
	 */

	std::stringstream sstr;
	sstr << argv[3] << argv[4];

	double kappa1, kappa2;
	sstr >> kappa1 >> kappa2;

	std::cout << kappa1 << std::endl;
	std::cout << kappa2 << std::endl;

	cv::Mat undistorted_image(image.size(), image.type());
	for ( int x = 0; x < image.cols; x++) {
		for (int y = 0; y < image.rows; y++) {
			double r = sqrt(pow(x - center.x,2) + pow(y - center.y, 2));
			double l_r = 1 + kappa1*r + kappa2*pow(r,2)/* + argv[5]*pow(r,3)*/; //nur 2. Ordnung...
			int x_new = center.x + l_r*(x-center.x);
			int y_new = center.y + l_r*(y-center.y);
			if (x_new < 0 || y_new < 0 || x_new >= image.cols || y_new >= image.rows)
				continue;
			cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x,y)); //pixel color from distorted image
			undistorted_image.at<cv::Vec3b>(cv::Point(x_new,y_new)) = color;
		}
	}
/* TODO */


	/**
	 * - das entzerrte Bild in einer Datei gespeichert wird. 
	 */
	cv::imwrite("./"+std::string(argv[2]), undistorted_image);

/* TODO */


	/**
	 * Was passiert, wenn die Größe der Parameter, ihr Vorzeichen etc. verändert wird?
	 * Ein Startwert kann z.B. $\kappa_1 = 0.001$, $\kappa_2 = 0.000005$ sein.
	 *
	 * \begin{figure}[hb]
	 * \centering
	 * \includegraphics[width=0.3\linewidth]{distorted}
	 * \caption{Bild mit starker radialer Verzerrung.}
	 * \label{fig:distorted}
	 * \end{figure}
	 */
	std::cin.get();
}

