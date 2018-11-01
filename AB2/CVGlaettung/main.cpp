#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

double kernelBinomial3x3[] = {
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
};


/*
>>> print(np.transpose(a) * a)
[[ 1  4  6  4  1]
 [ 4 16 24 16  4]
 [ 6 24 36 24  6]
 [ 4 16 24 16  4]
 [ 1  4  6  4  1]]
 */
double kernelBinomial5x5[] = {
    1.0, 4.0,  6.0,  4.0,  1.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    6.0, 24.0, 36.0, 24.0, 6.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    1.0, 4.0,  6.0,  4.0,  1.0,
};

constexpr double meanFactorValue = 0.1;

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "No image specified" << std::endl;
        return -1;
    }

    //Lade Bild im RGB Format
    cv::Mat inImage = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

    //Prüfe ob Bild geladen werden konnte
    if (!inImage.data)
    {
        std::cout << "Image has invalid format, couldn't load image" << std::endl;
        return -1;
    }

    //Benutzereingabe ob Binomialfilter Meanfilter
    char c;
    do {
        std::cout << "Choose [b]inomial or [m]ean filter:\t";
        std::cin >> c;
    } while ((c != 'b') && (c != 'm'));

    //Benutzereingabe ob 3x3 oder 5x5 Filterkern
    int blockSize;
    do {
        std::cout << "Choose [3]x3 or [5]x5 kernel matrix:\t";
        std::cin >> blockSize;
    } while ((blockSize != 3) && (blockSize != 5));

    //Erstelle Ausgabematrix
    cv::Mat outImage = inImage.clone(); //Ausgabematrix mit den gleichen Eigenschaften wie Eingangsmatrix wird benötigt

    double factor;
    std::cout << "Kernel matrix factor:\t";
    std::cin >> factor;

    //Lege Filterkern an
    cv::Mat kernelMatrix;
    if (c == 'b')
    {
        double* kernelValues;
        if (blockSize == 3)
            kernelValues = kernelBinomial3x3;
        else
            kernelValues = kernelBinomial5x5;

        kernelMatrix = cv::Mat(blockSize, blockSize, CV_64F, kernelValues).clone(); //Matrix kopieren, da die init Arrays sonst bei Multiplikation modifiziert wird
        kernelMatrix *= factor;
    }
    else
    {
        kernelMatrix = cv::Mat(blockSize, blockSize, CV_64F, factor);
    }

    std::cout << kernelMatrix << std::endl;

    //Wende Binomialfilter an
    cv::filter2D(inImage, outImage, -1, kernelMatrix);

    //Zeige Bild an
    //cv::namedWindow("Output", cv::WINDOW_NORMAL);
    cv::imshow("Output", outImage);
    cv::waitKey(0); //schließe Fenster, wenn eine Taste gedrückt wurde

    return 0;
}
