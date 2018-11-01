#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

constexpr unsigned c = 0;

int main(int argc, const char* argv[])
{
    //Wurde ein Dateiname für das Bild als Kommandozeilenargument übergeben worden? Wenn nicht, beenden
    if (argc < 2)
        return -1;

    //Bild als Graustufenbild laden
    cv::Mat inImage = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    //cout << image.cols << " " << image.rows << endl;

    //Prüfen ob Bild geladen oder Fehler aufgetreten ist
    if (inImage.data)
    {
        //Bild wurde geladen.

        //Kleinere Ausgabematrix anlegen (Format: Unsigned 8 Bit, 1 Kanal)
        cv::Mat outImage(inImage.rows - 2, inImage.cols - 2, CV_8U);

        //Über Pixel des Ausgabebildes iterieren:
        for (int x = 0; x < outImage.rows; x++)
        {
            for (int y = 0; y < outImage.cols; y++)
            {
                //(x,y) sind Koordinaten des Ausgabebildes

                //Umrechnung in Koordinaten des Ausgabebildes:
                int inputImageCoordX = x+1, inputImageCoordY = y+1;

                //Berechnung des Schwellwertes:
                unsigned threshold = (
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX-1, inputImageCoordY-1)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX,   inputImageCoordY-1)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX+1, inputImageCoordY-1)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX-1, inputImageCoordY)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX,   inputImageCoordY)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX+1, inputImageCoordY)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX-1, inputImageCoordY+1)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX,   inputImageCoordY+1)) +
                        static_cast<unsigned>(inImage.at<uchar>(inputImageCoordX+1, inputImageCoordY+1))
                        ) / 9 + c;

                //Binarisierung
                outImage.at<uchar>(x,y) = (inImage.at<uchar>(inputImageCoordX, inputImageCoordY) < threshold ? 0 : 255);
            }
        }

        //Ausgabe des Bildes, mache Fenster anpassbar (Größe)
        cv::namedWindow("Output", cv::WINDOW_NORMAL);
        cv::imshow("Output", outImage);
        cv::waitKey(0); //schließe Fenster, wenn eine Taste gedrückt wurde
    }

    return 0;
}
