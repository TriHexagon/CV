#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "No image file specified" << endl;
        return -1;
    }

    cv::Mat inImage = cv::imread(argv[1], 0);
    if (!inImage.data)
    {
        cerr << "Could not load image" << endl;
        return -1;
    }

    cv::Mat tmpImage;
    Canny(inImage, tmpImage, 1600, 3200, 5);
    //Canny(inImage, tmpImage, 150, 300, 3);
    //cv::cvtColor(tmpImage, tmpImage, CV_GRAY2BGR);

    vector<cv::Vec4i> lines;
    //cv::HoughLinesP(tmpImage, lines, 1, CV_PI/180, 25, 5, 10);
    //cv::HoughLinesP(tmpImage, lines, 1, CV_PI/180, 25);
    cv::HoughLinesP(tmpImage, lines, 1, CV_PI/180, 25, 5);

    cv::Scalar color(255, 255, 255);
    for (const auto& line : lines)
    {
        int x0 = line[0];
        int y0 = line[1];
        int x1 = line[2];
        int y1 = line[3];

        cv::Point pt1(x0, y0);
        cv::Point pt2(x1, y1);
        cv::line(inImage, pt1, pt2, color);
    }

    cv::namedWindow("Ausgabe", CV_WINDOW_NORMAL);
    cv::imshow("Ausgabe", inImage);
    cv::waitKey();

    return 0;
}
