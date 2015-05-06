/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 1.05.2015
 * Blatt: 2
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 8.5.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <iostream>
#include <cv.h>
#include <highgui.h>

/**
* http://stackoverflow.com/questions/22590907/cvsmooth-identifier-not-found ...
* http://answers.opencv.org/question/17546/opencv-will-drop-c-api-support-soon/
*/

int main(int argc, char **argv) {
	/**
	 * Aufgabe: 2D-Operationen auf Bildern (5 Punkte)
	 *
	 * In der Vorlesung wurde gezeigt, das große Bereiche natürlicher Bilder
	 * homogen sind, die meisten Informationen über ein Bild jedoch in den
	 * Kanten und Diskontinuitäten zu finden sind.  In disem Übungsblatt soll
	 * es darum gehen, Kanten und herausragende Punkte in Bildern zu finden.
	 * 
	 * Häufige Grundlage der Verarbeitung von Bildern ist das Anwenden von
	 * Filtern.  Es entspricht der Faltung (engl. \emph{convolution}) des
	 * Bildes mit einem Filterkern.  Filterkerne werden zu sehr verschiedenen
	 * Zwecken eingesetzt.
	 * 
	 * - Skizziere (auf Papier) eine eindimensionale Gaußfunktion mit
	 *   Mittelwert $\mu$ und Varianz $\sigma^2$.  Was ist die
	 *   Fourier-Transformation einer Gaußfunktion?
	 * - Lade ein Testbild und wandle es in ein Grauwertbild mit
	 *   float-Werten in $[0, 1]$ um.
	 */

	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
		exit(1);
	}
	IplImage *image = cvLoadImage(argv[1]);
	CvSize size = cvGetSize(image);
	IplImage *imageFloat = cvCreateImage(size, IPL_DEPTH_32F, image->nChannels);
	IplImage *gray = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	cvConvertScale(image, imageFloat, 1.0 / 255.0);
	cvCvtColor(imageFloat, gray, CV_BGR2GRAY);

	//cvNamedWindow("Grayscale"); 
	//cvShowImage("Grayscale", gray);
	//cvWaitKey(0); 

	/**
	 * - Falte ein verrauschtes Testbild mit Gaußfunktionen verschiedener
	 *   Varianzen. Was passiert? Welchen Einfluss hat die Kernelgröße?
	 */
	IplImage *lgauss = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage *sgauss = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	int kernel_size = 201;
	float sigma_s = 2;
	float sigma_l = 20;
	cvSmooth(gray, sgauss, CV_GAUSSIAN, kernel_size, kernel_size, sigma_s);
	cvSmooth(gray, lgauss, CV_GAUSSIAN, kernel_size, kernel_size, sigma_l);

	//cvNamedWindow("Large Gaussian"); 
	//cvShowImage("Large Gaussian", lgauss);
	//cvNamedWindow("Small Gaussian"); 
	//cvShowImage("Small Gaussian", sgauss);
	//cvWaitKey(0); 

	/**
	 * - Betrachte die Differenzen zweier gaußgefilterter Bilder (evt.
	 *   skalieren).
	 */
	IplImage *dog = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	cvAbsDiff(lgauss, sgauss, dog);

/*	cvNamedWindow("Difference of Gaussians"); 
	cvShowImage("Difference of Gaussians", dog);
	cvWaitKey(0);*/ 

	/**
	 * Aufgabe: Diskrete Ableitungen (5 Punkte)
	 *
	 * Mathematisch sind Ableitungen nur für stetige Funktionen definiert.  Da
	 * ein Bild nur pixelweise, d.h. an diskreten Stellen, Informationen
	 * liefert, kann man Ableitungen von Bildern nicht direkt bestimmen.  Eine
	 * naheliegene Approximation der Ableitung ist der Differenzenquotient.
	 * Sei $f:\Omega \subset \mathbb{R} \to \mathbb{R}$ eine Funktion.  Dann
	 * ist der Differenzenquotient $D_h(x) = \frac{f(x+h) - f(x)}{h}$ eine
	 * Approximation an $f'(x)$ für hinreichend kleines h.  Für
	 * differenzierbare Funktionen liefert allerdings die zentrale Differenz
	 * \begin{equation}
	 * D(x) = \frac{f(x+h) - f(x-h)}{2h}
	 * \end{equation}
	 * eine deutlich bessere Approximation (siehe auch \emph{Numerical Recipes
	 * in C} von Press et al., Chapter 5).
	 *
	 * - Bestimme einen diskreten Faltungskern, der die zentrale Differenz
	 *   approximiert.
	 */

/* TODO */
	float kernel[] = { -10.0, 0, 10.0 };
	CvMat *filterDx = cvCreateMatHeader(1, 3, CV_32FC1);
	CvMat *filterDy = cvCreateMatHeader(3, 1, CV_32FC1);
	cvSetData(filterDx, kernel, filterDx->step);
	cvSetData(filterDy, kernel, filterDy->step);

	/**
	 * - Implementiere diskretes Differenzieren als Faltung mit diesem Kern und
	 *   wende es auf ein glattes Testbild an.  Was passiert, wenn du ein
	 *   verrauschtes Testbild verwendest?
	 */
	IplImage *dx = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage *dy = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	cvFilter2D(gray, dx, filterDx);
	cvFilter2D(gray, dy, filterDy);

	//cvNamedWindow("DX"); 
	//cvShowImage("DX", dx);
	//cvNamedWindow("DY"); 
	//cvShowImage("DY", dy);
	//cvWaitKey(0); 

	/**
	 * - Verwende in der Implementierung nun Faltung mit dem Sobel-Operator
	 *   (\code{cvSobel}) und beobachte die Ergebnisse auf dem verrauschten
	 *   Testbild.
	 */
	IplImage *sobel = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	cvSobel(gray, sobel, 1, 0);

	cvNamedWindow("sobel"); 
	//cvShowImage("sobel", sobel);
	//cvWaitKey(0);

	/**
	 * Aufgabe: Features (10 Punkte)
	 *
	 * Kanten in Bildern werden häufig als Intensitätssprünge beschrieben.
	 *
	 * - Berechne den Betrag des Gradienten eines Testbildes und bestimme
	 *   Schwellwerte des Gradienten, um möglichst alle Kanten zu entdecken
	 *   oder möglichst nur Kanten zu entdecken.
	 */
	IplImage* gradient = cvCreateImage(size, IPL_DEPTH_32F, 1);

/* TODO */
	IplImage *dxx = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage *dyy = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvMul(dx, dx, dxx);
	cvMul(dy, dy, dyy);

	cvAdd(dxx, dyy, gradient);

	//cvShowImage("Gradient", gradient);
	//cvWaitKey(0);

	IplImage* edge1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* edge2 = cvCreateImage(size, IPL_DEPTH_8U, 1);

/* TODO */
	int t1 = 1, t2 = 10;
	cvCmpS(gradient, t1, edge1, CV_CMP_GT);
	cvCmpS(gradient, t2, edge2, CV_CMP_GT);

	/*cvShowImage("All edges", edge1);
	cvWaitKey(0);
	cvShowImage("Only edges", edge2);
	cvWaitKey(0);*/

	/*
	 * - Vergleiche mit dem Ergebnis des Canny-Kantendetektors
	 *   (\code{cvCanny}), wenn er mit diesen Parametern aufgerufen wird.
	 */
	IplImage* edge = cvCreateImage(size, IPL_DEPTH_8U, 1);

/* TODO */
	IplImage* gray_u8 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvCvtColor(image, gray_u8, CV_BGR2GRAY);
	cvCanny(gray_u8, edge, t1, t2, 3);

	//cvShowImage("Canny", edge);
	//cvWaitKey(0);

	/**
	 * Einzelne herausragende Punkte werden auch als Featurepunkte oder Ecken
	 * bezeichnet, selbst wenn sie nicht auf einer Kante liegen.
	 *
	 * - Implementiere die Harris-Corner Detektion.  Verwende dabei nicht die
	 *   OpenCV Methode cvCornerHarris, sondern implementiere selbst eine
	 *   Funktion, die ein Grauwertbild, einen Parameter $k$ für die Berechnung
	 *   des Featureindikators und einen Schwellwert für ausreichend großen
	 *   Ausschlag des Indikators entgegennimmt und die Featurepunkte
	 *   zurückgibt.
	 */
	std::vector<CvPoint> harris_pts;

/* TODO */
    IplImage* i_xx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* i_yy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* i_xy = cvCreateImage(size,IPL_DEPTH_32F,1);
	
    cvSobel(gray_u8,dx,1,0,3);
    cvSobel(gray_u8,dy,0,1,3);

    cvMul(dx,dx,i_xx,1);
    cvMul(dy,dy,i_yy,1);
    cvMul(dx,dy,i_xy,1);

	//or 5?
	cvSmooth(i_xx, i_xx, CV_BLUR, 3, 3);
	cvSmooth(i_yy, i_yy, CV_BLUR, 3, 3);
	cvSmooth(i_xy, i_xy, CV_BLUR, 3, 3);

    double k = 0.04, omega = 0.1;
    for(int x = 0; x < size.width; x++){
        for(int y=0; y < size.height; y++){
            CvMat* A = cvCreateMat(2,2,CV_32FC1);
			
			float d00 = CV_IMAGE_ELEM(i_xx, float, y, x); 
			float d01 = CV_IMAGE_ELEM(i_xy, float, y, x); 
			float d10 = CV_IMAGE_ELEM(i_xy, float, y, x); 
			float d11 = CV_IMAGE_ELEM(i_yy, float, y, x); 
            
			cvSet2D(A,0,0,cvRealScalar(d00));
            cvSet2D(A,0,1,cvRealScalar(d01));
            cvSet2D(A,1,0,cvRealScalar(d10));
            cvSet2D(A,1,1,cvRealScalar(d11));

            double R = cvDet(A)-k* (CV_MAT_ELEM((*A),float,0,0)+CV_MAT_ELEM((*A),float,1,1));

            if(R>10000.0){
				/*
				missing test for local maximum...
				therefore response function has probably to be outside of loop and calculated directly...
				will do that later...
				*/
                harris_pts.push_back(cvPoint(x,y));
            }

			//std::cout << R << std::endl;
        }
    }
	/**
	 * - Zeichne ein $3\times3$ Rechteck um jede gefundene Harris-Corner.
	 */

/* TODO */

	for (int j = 0; j<harris_pts.size();j++){
        int x = harris_pts[j].x;
        int y = harris_pts[j].y;
        cvRectangle(image, cvPoint(1+x, 1+y) , cvPoint(-1+x,-1+y) , cvScalar(0, 0, 255, 0), 1, 8, 0);
    }

	cvShowImage("Harris Corners", image);
	cvWaitKey(0);
}

