#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main()
{
    cv::Mat refImage = cv::imread("../../../objektRef.png", cv::IMREAD_GRAYSCALE);
    cv::Mat searchImage = cv::imread("../../../objekte.png", cv::IMREAD_GRAYSCALE);

    if (refImage.empty() || searchImage.empty())
    {
        cerr << "Failed to load images" << endl;
        return -1;
    }

    //cv::imshow("Ausgabe1", refImage);
    //cv::imshow("Ausgabe2", searchImage);
    //cv::waitKey(0);

    std::vector<cv::Vec4f> positions;
    //std::unique_ptr<cv::GeneralizedHoughBallard> hough = std::unique_ptr<cv::GeneralizedHoughBallard>(cv::createGeneralizedHoughBallard());
    cv::Ptr<cv::GeneralizedHoughBallard> hough = cv::createGeneralizedHoughBallard();
    hough->setMinDist(100);  //minimum distance between the centers of the detected objects
    hough->setLevels(360);  //R-Table levels
    hough->setDp(2.0);      // inverse ratio of the accumulator resolution to the image resolution
    hough->setMaxBufferSize(1024*1024); //maximal size of inner buffers
    hough->setVotesThreshold(40);       //position votes threshold

    hough->setTemplate(refImage);
    hough->detect(searchImage, positions);

    if (positions.size() == 0)
        cout << "Nothing found" << endl;

    for (const auto& i : positions)
    {
        int x = i[0], y = i[1];
        cout << "obj found x=" << x << " y=" << y
             << " scale=" << i[2] << " angle=" << i[3] << endl;

        cv::line(searchImage, cv::Point(x-5, y-5), cv::Point(x+5, y+5), cv::Scalar(0, 0), 3);
        cv::line(searchImage, cv::Point(x+5, y-5), cv::Point(x-5, y+5), cv::Scalar(0, 0), 3);
    }

    cv::imshow("Ausgabe", searchImage);
    cv::waitKey();

    return 0;
}
