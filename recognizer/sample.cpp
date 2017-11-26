#include "sample.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "define.h"
#include <QFile>
#include <QDebug>

using namespace cv;
using namespace std;

Sample::Sample()
{
    initFlagContour();
}

Sample& Sample::instance()
{
    static Sample s_instance;
    return s_instance;
}

double Sample::compareFlagContour(std::vector<cv::Point> contour)
{
    return matchShapes(contour, _flagContour, CV_CONTOURS_MATCH_I1, 0.0);//形状匹配:值越小越相似
}

void Sample::initFlagContour()
{
    // findContours supports CV_8UC1
    auto filepath = SPATH "flag_contour.png";
    if (!QFile::exists(filepath)) {
        qDebug() << "err: file not exists: " << filepath;
        return;
    }

    Mat bin = imread(filepath, CV_8UC1);
    imshow("flag_contour", bin);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(bin, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Q_ASSERT(contours.size() == 1);
    _flagContour = contours[0];
}

