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

    int rows = images.size();

    for(int y = 0; y < images[0]->height; ++y) {
        for(int x = 0; x < images[0]->width; ++x) {
            cv::Mat light_mat(rows, 3, CV_32FC1);
            cv::Mat intensity_mat(rows, 1, CV_32FC1);

            for(int i = 0; i < images.size(); ++i) {
                cv::Mat img = images[i];
                int intensity_char = img.at<unsigned char>(y, x);
                float intensity = intensity_char / 255.0;

                intensity_mat.at<float>(i) = intensity;

                light_mat.at<float>(i, 0) = lights[i].x;
                light_mat.at<float>(i, 1) = lights[i].y;
                light_mat.at<float>(i, 2) = lights[i].z;
            }

            cv::Mat np(3, 1, CV_32FC1, cv::Scalar(0));
            cv::solve(light_mat, intensity_mat, np, cv::DECOMP_SVD);

            normalize(np, np);
            cv::Vec3f nvec(np);

            //std::cout << nvec << std::endl;
            n.at<cv::Vec3f>(y, x) = nvec;
        }
    }

    cv::namedWindow("normals", CV_WINDOW_AUTOSIZE);
    //cv::Mat channels[3];
    //cv::split(n, channels);
    //cv::Mat neworder[] = {channels[2], channels[0], channels[1]};

    //cv::merge(neworder, 3, n);
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

    cv::Mat h(heights);
    cv::Mat nor(normals);

    for (int i = 0; i < num_steps; ++i) {
        cv::Mat newh = cv::Mat(h.rows, h.cols, CV_32FC1, cv::Scalar(0, 0, 0));
        for (int y = 1; y < h.rows - 2; ++y) {
            for (int x = 1; x < h.cols - 2; ++x) {
                cv::Vec3f n = nor.at<cv::Vec3f>(y, x);
                float nxz = n[0] / n[2];
                float nyz = n[1] / n[2];
                nxz = std::isnan(nxz) ? 0 : nxz;
                nyz = std::isnan(nyz) ? 0 : nyz;
                nxz = std::isinf(nxz) ? 0 : nxz;
                nyz = std::isinf(nyz) ? 0 : nyz;
                float z = h.at<float>(y, x);
                float z_right = h.at<float>(y, x + 1);
                float z_top = h.at<float>(y + 1, x);

                newh.at<float>(y, x) += z_right - nxz;
                newh.at<float>(y, x) += z_top - nyz;
                newh.at<float>(y, x + 1) += nxz + z;
                newh.at<float>(y + 1, x) += nyz + z;
            }
        }

        for (int y = 0; y < h.rows; ++y) {
            for (int x = 0; x < h.cols; ++x) {
                h.at<float>(y, x) = newh.at<float>(y, x) / 4;
            }
        }
    }

    std::cout << h << std::endl;

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
        IplImage *gimage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
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

