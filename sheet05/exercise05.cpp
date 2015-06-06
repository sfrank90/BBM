/**
 * Vorlesung: Bildbasierte Modellierung SS 2012
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 05.06.2015
 * Blatt: 5
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 12.06.2012.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <limits>

#include <cv.h>
#include <highgui.h>

using namespace std;

class SIFTFeature {
  public:
    //Pixel position 
    float x;
    float y;

    //Scale and orientation of the sift feature
    float scale;
    float orientation;

    //Descritor 
    float descriptor[128];

    CvPoint2D32f getPos() const {
      return cvPoint2D32f(x, y);
    }
};

/*
 * Takes two images (left and middle) and a Homography to 
 * create a Panorama.
 * 
 * The Panorama is then displayed.
 */
void createPanorama(const IplImage *img_l, const IplImage *img_m, CvMat* P) {
	//Determine size of new image
	int h = img_l->height - 1;
	int w = img_l->width - 1;
	float z1[] = {0.0, 0.0, 1.0 };
	float z2[] = {0.0, (float)(h), 1.0 };
	float z3[] = {(float)(w), (float)(h), 1.0 };
	float z4[] = {(float)(w), 0.0, 1.0 };

	CvMat* Z1 = cvCreateMatHeader(3,1, CV_32FC1);
	cvSetData( Z1, z1, Z1->step );
	CvMat* Z2 = cvCreateMatHeader(3,1, CV_32FC1);
	cvSetData( Z2, z2, Z2->step );
	CvMat* Z3 = cvCreateMatHeader(3,1, CV_32FC1);
	cvSetData( Z3, z3, Z3->step );
	CvMat* Z4 = cvCreateMatHeader(3,1, CV_32FC1);
	cvSetData( Z4, z4, Z4->step );

	cvMatMul(P, Z1, Z1);
	cvMatMul(P, Z2, Z2);
	cvMatMul(P, Z3, Z3);
	cvMatMul(P, Z4, Z4);

	float z1_z = cvGetReal1D( Z1, 2);
	float z2_z = cvGetReal1D( Z2, 2);
	float z3_z = cvGetReal1D( Z3, 2);
	float z4_z = cvGetReal1D( Z4, 2);

	if ( (z1_z==0) || (z2_z==0) || (z4_z==0) || (z4_z==0)) {
		throw runtime_error("Invalid homography P specified!");
	}

	cvScale(Z1, Z1, 1.0 / z1_z);
	cvScale(Z2, Z2, 1.0 / z2_z);
	cvScale(Z3, Z3, 1.0 / z3_z);
	cvScale(Z4, Z4, 1.0 / z4_z);

	CvPoint offmin, offmax;
	offmin.x = floor( min( min( min( cvmGet(Z1,0,0), cvmGet(Z2,0,0) ), cvmGet(Z3,0,0) ), cvmGet(Z4,0,0) ) );
	offmin.x = min(0, offmin.x);
	offmin.y = floor( min( min( min( cvmGet(Z1,1,0), cvmGet(Z2,1,0) ), cvmGet(Z3,1,0) ), cvmGet(Z4,1,0) ) );
	offmin.y = min(0, offmin.y);

	offmax.x = ceil( max( max( max( cvmGet(Z1,0,0), cvmGet(Z2,0,0) ), cvmGet(Z3,0,0) ), cvmGet(Z4,0,0) ) );
	offmax.x = max( w, offmax.x);
	offmax.y = ceil( max( max( max( cvmGet(Z1,1,0), cvmGet(Z2,1,0) ), cvmGet(Z3,1,0) ), cvmGet(Z4,1,0) ) );
	offmax.y = max( h, offmax.y);

	CvSize s = cvSize(offmax.x - offmin.x, offmax.y - offmin.y);

	//Fill the new-size images with data
	IplImage *Panorama = cvCreateImage(s, IPL_DEPTH_32F, img_l->nChannels);
	IplImage *P1 = cvCreateImage(s, IPL_DEPTH_32F, img_l->nChannels);
	IplImage *P2 = cvCreateImage(s, IPL_DEPTH_32F, img_l->nChannels);

	cvZero(P1);
	cvZero(P2);
	cvZero(Panorama);

	cvSetImageROI(P1, cvRect(0, 0, img_l->width, img_l->height));
	cvSetImageROI(P2, cvRect(0, 0, img_m->width, img_m->height));

	cvCopy(img_l, P1);
	cvCopy(img_m, P2);

	cvResetImageROI(P1);
	cvResetImageROI(P2);

	cvShowImage("mainWin", P2 );
	cvWaitKey(0);
	cvShowImage("mainWin", P1 );
	cvWaitKey(0);


	//Determine projective transformation relative to new image size
	CvMat* T = cvCreateMatHeader(3, 3, CV_32FC1);
	float rev_trans[] = { 1.0, 0.0, -offmin.x, 0.0, 1.0, -offmin.y, 0.0, 0.0, 1.0};

	cvSetData( T, rev_trans, T->step);

	cvMatMul(T,P,P);

	cout << "Warp now" << endl;
	//Create the panorama
	cvWarpPerspective( P1, Panorama, P );
	cvWarpPerspective( P2, P1, T);
	cvCopy( P1, P2);

	IplImage* MaskL = cvCreateImage( s, IPL_DEPTH_32F, P1->nChannels);
	IplImage* MaskM = cvCreateImage( s, IPL_DEPTH_32F, P1->nChannels);

	cvThreshold(Panorama, MaskL, 0.001, 1.0, CV_THRESH_BINARY );
	cvThreshold(P2,       MaskM, 0.001, 1.0, CV_THRESH_BINARY );

	cvAdd( Panorama, P2, Panorama );
	cvAdd( MaskL, MaskM, MaskM );
	cvDiv( Panorama, MaskM, Panorama);

	cvShowImage("mainWin", Panorama );
	cvWaitKey(0);

	cvReleaseMat( &Z1);
	cvReleaseMat( &Z2);
	cvReleaseMat( &Z3);
	cvReleaseMat( &Z4);
	cvReleaseMat( &T);
	cvReleaseImage( &P1);
	cvReleaseImage( &P2);
	cvReleaseImage( &Panorama);
	cvReleaseImage( &MaskL);
	cvReleaseImage( &MaskM);
}

/*
 * Read a key file generated by the Lowes sift binary
 * and return a vector of the contained features.
 */
std::vector<SIFTFeature> readSIFT(const std::string &filename) {
	vector<SIFTFeature> features;
	fstream file(filename.c_str(), ios_base::in);

	if (file.bad() || file.eof()) {
		cerr << "Could not open file: " << filename << endl;
		exit(0);
	}

	unsigned int num, len;

	file >> num;
	file >> len;

	cout << "Reading " << num << " descriptors" << endl;

	if (len != 128) {
		cerr << "Keypoint descriptor length invalid (should be 128)." << endl;
		exit(0);
	}

	for (unsigned int i = 0; i < num; ++i) {
		SIFTFeature feat;
		file >> feat.y;
		file >> feat.x;
		file >> feat.scale;
		file >> feat.orientation;

		for (unsigned int d = 0; d < len; ++d) {
			file >> feat.descriptor[d];
			feat.descriptor[d] /= 255.0;

		}
		features.push_back(feat);
	}

	return features;
}

float compareDescriptors(const SIFTFeature &f1, const SIFTFeature &f2) {
	float d = 0.0;
	for (unsigned int i = 0; i < 128; ++i) {
		d += (f1.descriptor[i] - f2.descriptor[i]) * (f1.descriptor[i] - f2.descriptor[i]);
	}
	return d;
}

/**
 * Aufgabe: SIFT-Featurepunkte (5 Punkte)
 *
 * Auf seiner Webseite \url{http://www.cs.ubc.ca/~lowe/keypoints/} stellt
 * Lowe, der Autor der SIFT Features, ein Programm zum Bestimmen der
 * SIFT-Features und zum Berechnen der SIFT-Deskriptoren für Forschungszwecke
 * zur Verfügung.
 * 
 * - Berechne mit der Implementation von Lowe für die Testbilder
 *   SIFT-Featurepunkte und lasse die Deskriptoren in einer Datei abspeichern.
 * - Implementiere das Finden von Korrespondenzen zwischen Paaren von
 *   Testbildern. Lowe gibt an, dass ein Match nur akzeptiert werden soll, wenn
 *   die beste Deskriptordistanz kleiner ist als das $0.6$-fache der
 *   zweitbesten Deskriptordistanz. Implementiere diese Prüfung. Was passiert,
 *   wenn der Faktor auf $0.4$ bzw. $0.8$ gesetzt wird?
 */
int matchDescriptors(const SIFTFeature &feat, const vector<SIFTFeature> &featlist, float ratio) {
	/*
	 * This code matches the descriptor feat (1st Parameter) against
	 * all descriptors in featlist (2nd Parameter).
	 * When a match is found the index within the featlist vector is returned.
	 * When no match is found -1 is returned.
	 */

/* TODO */

}

/**
 * Aufgabe: RANSAC (15 Punkte)
 * 
 * Tatsächlich liefern selbst sehr gute Matching-Algorithmen stets einige
 * Korrespondenzen, die nicht denselben 3D Punkt beschreiben. Homographien, die
 * auf diesen Korrespondenzen beruhen, liefern nicht die gewünschten
 * Transformationen. Der RANSAC-Algorithmus versucht, aus der Gesamtmenge der
 * Korrespondenzen diejenigen herauszusuchen, die zueinander konsistent sind.
 *
 * - Implementiere den RANSAC-Algorithmus. Verwende als Qualitätsmaß den
 *   Abstand zwischen den Featurepunkten des einen Bildes und den
 *   transformierten Featurepunkten des anderen Bildes: Abweichungen von mehr
 *   als vier Pixeln in einer Koordinate kennzeichnen einen Punkt als
 *   Abweichung. Bei gleicher Anzahl konsistenter Korrespondenzen entscheidet
 *   die Gesamtabweichung aller gültigen Korrespondenzen.
 */
void RANSACTransform(const unsigned int &ransac_iterations, const vector<SIFTFeature> &keypoints1, const vector<SIFTFeature> &keypoints2, 
		const vector<pair<unsigned int, unsigned int> > &ptpairs, CvMat *P_in) {

/* TODO */


	/**
	 * - Berechne mit allen gültigen Korrespondenzen eine Homographie zwischen
	 *   den Bildern. 
	 * - Stelle mit dieser Homographie ein Panorama her.
	 */

/* TODO */

}

void findPairs(const vector<SIFTFeature> &keypoints1, const vector<SIFTFeature> &keypoints2, 
		vector<pair<unsigned int, unsigned int> > &ptpairs, float ratio = 0.6) {
	ptpairs.clear();
	for(unsigned int i = 0; i < keypoints1.size(); ++i) {
		int match = matchDescriptors(keypoints1[i], keypoints2, ratio);
		if (match >= 0) {
			ptpairs.push_back(make_pair(i, static_cast<unsigned int>(match)));
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc <= 5) {
		cout << "Usage: main <image-file-name1> <image-file-name2> <keyfile1> <keyfile2> <ratio>" << endl;
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
		exit(0);
	}

	float ratio = atof(argv[5]);

	//Convert to suitable format
	IplImage* img1f = cvCreateImage( cvGetSize(img1), IPL_DEPTH_32F, img1->nChannels);
	IplImage* img2f = cvCreateImage( cvGetSize(img2), IPL_DEPTH_32F, img1->nChannels);

	cvConvertScale(img1, img1f, 1.0/255.0);
	cvConvertScale(img2, img2f, 1.0/255.0);

	//Load Features

	vector<SIFTFeature> keypoints1 = readSIFT(argv[3]);
	vector<SIFTFeature> keypoints2 = readSIFT(argv[4]);

	//Match Descriptors
	vector<pair<unsigned int, unsigned int> > ptpairs;
	findPairs(keypoints1, keypoints2, ptpairs, ratio);
	cout << "Found " << ptpairs.size() << " matches " << endl;

	//Show the matched feature points
	CvSize s = cvGetSize(img1);
	s.width += img2->width;
	IplImage *img_match = cvCreateImage(s, IPL_DEPTH_32F, img1->nChannels);

	cvSetImageROI(img_match, cvRect(0, 0, img1->width, img1->height));
	cvCopy(img1f, img_match);
	cvSetImageROI(img_match, cvRect(img1->width, 0, img2->width, img2->height));
	cvCopy(img2f, img_match);
	cvResetImageROI(img_match);

	CvScalar color = cvScalar(1,0,0);

	vector<pair<unsigned int, unsigned int> >::const_iterator it = ptpairs.begin();
	for (;it != ptpairs.end(); ++it) {
		const SIFTFeature &f1 = keypoints1[it->first];
		const SIFTFeature &f2 = keypoints2[it->second];

		CvPoint pt1, pt2;
		pt1.x = f1.x;
		pt1.y = f1.y;
		pt2.x = f2.x + img1f->width;
		pt2.y = f2.y;
		cvLine(img_match, pt1, pt2, color);
	}

	cvShowImage("mainWin", img_match );
	cvWaitKey(0);

	//Calulate best projective transform with RANSAC
	CvMat* P = cvCreateMat(3, 3, CV_32FC1);
	RANSACTransform(100, keypoints1, keypoints2, ptpairs, P);
	createPanorama(img1f, img2f, P);

	// destroy the window
	cvDestroyWindow( "mainWin" );

	// tidy up
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img1f);
	cvReleaseImage(&img2f);
	cvReleaseImage(&img_match);
	cvReleaseMat(&P);

	return 0;
}

