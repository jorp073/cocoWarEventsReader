#include "wareventrecognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "util/util.h"
#include <QDebug>

using namespace cv;

WarEventRecognizer::WarEventRecognizer()
{

}

bool WarEventRecognizer::getWarEventData(cv::Mat img)
{
    return false;
}

void WarEventRecognizer::findWarEventBars(cv::Mat img, std::vector<cv::Mat> &bars)
{
    bars.clear();

    qDebug() << "img: " << img.cols << "x" << img.rows;
    imshow("img", img);

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    imshow("gray", gray);

    Mat bin;
    Util::thresholdMinMax(gray, bin, 221, 243);
    imshow("bin", bin);

    std::vector<std::vector<Rect> > cellRects;
    Util::getCellRectWithoutBorder(bin, 0.6f, 0.7f, 0.6f, 52.0f/905, cellRects);

    for (auto const & rows : cellRects) {
        for (auto const & rect : rows) {
            Mat tmp(bin, rect);
            imshow("tmp", tmp);
            waitKey(0);
        }
    }

    waitKey(0);
}
