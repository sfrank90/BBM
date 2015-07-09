/**
 * Vorlesung: Bildbasierte Modellierung SS 2012
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 05.07.2015
 * Blatt: 9
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 10.07.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss12/bbm/
 */

/**
 * In dieser Aufgabe soll ein einfacher Shape-from-Shading-Algorithmus
 * implementiert werden. Gegeben sind mehrere Bilder eines diffusen Objekts aus
 * derselben Kameraperspektive, aber unter Beleuchtung aus unterschiedlichen
 * (bekannten) Richtungen. Daraus können die Normalen der Geometrie
 * rekonstruiert werden, die anschließend zu einer Tiefenkarte aufintegriert
 * werden.
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

/**
 * Aufgabe: Normalen berechnen (10 Punkte)
 *
 * Für die Intensität $I$, die von einer diffusen, aus Richtung $\vec{l}$
 * beleuchteten Oberfläche mit Normale $\vec{n}$ zurückgeworfen wird, gilt $I
 * \propto \vec{l} \cdot \vec{n}$. Jedes Eingabebild definiert $I$ und
 * $\vec{l}$ für jeden Pixel ($\vec{l}$ ist konstant über alle Pixel).
 *
 * - Stelle ein lineares Gleichungssystem auf, das obigen Zusammenhang für
 *   einen Pixel in allen Bildern beschreibt.
 *
 * - Löse das Gleichungssystem für jeden Pixel, z.B. mittels \code{cvSolve}.
 * 
 * - Zeige die resultierenden Normalen als Farbbild an und prüfe sie auf
 *   Plausibilität.
 */
IplImage *compute_normals(const std::vector<IplImage *> &images,
                          const std::vector<CvPoint3D32f> &lights) {
    IplImage *normals = cvCreateImage(cvSize(images[0]->width,
                                             images[0]->height),
                                      IPL_DEPTH_32F, 3);
    cv::Mat n(normals);

    int cols = images.size();

    for(int y = 0; y < images[0]->height; ++y) {
        for(int x = 0; x < images[0]->width; ++x) {
            cv::Mat light_mat(cols, 3, CV_32FC1);
            cv::Mat intensity_mat(cols, 1, CV_32FC1);
            for(int i = 0; i < images.size(); ++i) {
                cv::Mat img = images[i];
                intensity_mat.at<float>(i) = img.at<float>(y, x);

                light_mat.at<float>(0, i) = lights[i].x;
                light_mat.at<float>(1, i) = lights[i].y;
                light_mat.at<float>(2, i) = lights[i].z;
            }

            cv::Mat np(3, 1, CV_32FC1);
            cv::solve(light_mat, intensity_mat, np, cv::DECOMP_SVD);

            n.at<cv::Vec3f>(y, x) = np;
        }
    }

    cv::namedWindow("normals", CV_WINDOW_AUTOSIZE);
    cv::imshow("normals", n);
    cv::waitKey(0);

    return normals;
}

/**
 * Aufgabe: Tiefenkarte berechnen (20 Punkte)
 *
 * Um aus den Normalen eine konsistente Geometrie zu berechnen, muss eine
 * Tiefenkarte gefunden werden, die genau diese Normalen aufweist. Ausgehend
 * von einer flachen Tiefenkarte wird jeder Pixel an den Punkt verschoben, der
 * die Abweichung der tatsächlichen Normalen an dieser Stelle von der in der
 * vorhergehenden Aufgabe berechneten Normalen minimiert. Dieser Vorgang wird
 * wiederholt, bis Konvergenz eintritt.
 *
 * - Aus der Normalen lassen sich diskretisierte Ableitungen der Tiefenkarte
 *   $z$ berechnen: $$z(x+1, y) - z(x, y) = \frac{n_x(x, y)}{n_z(x, y)}
 *   \quad\text{sowie}\quad z(x, y+1) - z(x, y) = \frac{n_y(x, y)}{n_z(x, y)}$$
 *   Löse diese Gleichungen nach $z(x, y)$, $z(x+1, y)$ bzw. $z(x, y+1)$ auf.
 *   Dies liefert Sollwerte, die $z(x, y)$, $z(x+1, y)$ bzw. $z(x, y+1)$ gemäß
 *   $\vec{n}(x, y)$ und den benachbarten $z$-Werten haben müssten.
 *
 * - Implementiere ein iteratives Lösungsverfahren für die Tiefenkarte, indem
 *   du in jedem Schritt jeden Tiefenwert durch den Mittelwert seiner Sollwerte
 *   ersetzt.
 *
 * - Implementiere die Randbedingung, dass die Ränder der Tiefenkarte den Wert
 *   null haben sollen, indem du sie während der Iterationen auf diesem Wert
 *   festhältst.
 */
IplImage *integrate_normals(IplImage *normals, int num_steps=100) {
    IplImage *heights = cvCreateImage(cvSize(normals->width, normals->height), IPL_DEPTH_32F, 1);
	cvSet(heights, cvScalar(0));

/* TODO */

    return heights;
}

CvPoint3D32f angle_from_filename(const char *filename) {
	const char *s0 = strrchr(filename, '/');
	if (s0 == NULL) s0 = filename;
	const char *s1 = strchr(s0 + 1, '_');
	const char *s2 = strchr(s1 + 1, '_');
	const char *s3 = strchr(s2 + 1, '.');

	char a[s2 - s1];
	strncpy(a, s1 + 1, s2 - s1 - 1);
	a[s2 - s1 - 1] = 0;
	float ax = atof(a);

	char b[s3 - s2];
	strncpy(b, s2 + 1, s3 - s2 - 1);
	b[s3 - s2 - 1] = 0;
	float ay = atof(b);

    float sx = sin(ax / 180.0 * M_PI);
    float cx = cos(ax / 180.0 * M_PI);
    float sy = sin(ay / 180.0 * M_PI);
    float cy = cos(ay / 180.0 * M_PI);
    return cvPoint3D32f(sx, cx * sy, cx * cy);
}

int main(int argc, const char *argv[]) {
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] << " list_of_image_filenames" << std::endl;
		return 1;
	}

    std::vector<IplImage *> images;
    std::vector<CvPoint3D32f> lights;
    for (int i = 1; i < argc; ++i) {
        IplImage *image = cvLoadImage(argv[i]);
        IplImage *gimage = cvCreateImage(cvSize(image->width, image->height), 8, 1);
        cvCvtColor(image, gimage, CV_RGB2GRAY);
        cvReleaseImage(&image);
        images.push_back(gimage);
        lights.push_back(angle_from_filename(argv[i]));
    }

	IplImage *normals = compute_normals(images, lights);
	cvShowImage("normals", normals);
	IplImage *heights = integrate_normals(normals);
	double minval, maxval;
	cvMinMaxLoc(heights, &minval, &maxval);
	cvCvtScale(heights, heights, 1.0, -minval);
	cvCvtScale(heights, heights, 1.0 / (maxval - minval));
	cvShowImage("heights", heights);
	cvWaitKey(0);

    return 0;
}

