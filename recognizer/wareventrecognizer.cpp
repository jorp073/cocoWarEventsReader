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

bool WarEventRecognizer::getData(cv::Mat img)
{
    WarEventRecognizer rec;

    std::vector<Mat> bars;
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

    for (auto const & bar : bars) {
        // if (!isAttack(bar)) continue;

        if (parseBar(bar)) {

        }
    }

    return true;
}

void WarEventRecognizer::findBars(cv::Mat img, std::vector<cv::Mat> &bars)
{
    bars.clear();

    //imshow("img", img);

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    //imshow("gray", gray);

    Mat bin;
    Util::thresholdMinMax(gray, bin, 221, 243);
    //imshow("bin", bin);

    std::vector<std::vector<Rect> > cellRects;
    Util::getCellRectWithoutBorder(bin, 0.6f, 0.7f, 0.6f, 52.0f/905, cellRects);

    for (size_t row=1; row<cellRects.size(); row++) {
        for (auto const & rect : cellRects[row]) {
            Mat bar(img, rect);
            // remove rect top border part
            const int top = 0.13f * bar.rows;
            const int bottom = 0.13f * bar.rows;
            const int left = 0.02f * bar.cols;
            const int right = 0.02f * bar.cols;
            bar = Mat(bar, Rect(left, top, bar.cols - left - right, bar.rows - top - bottom));
            bars.push_back(bar);

            imshow(QString("bar%1").arg(row-1).toStdString(), bar);
        }
    }
}

bool WarEventRecognizer::isAttack(Mat bar)
{
    int w = bar.cols*0.1f;
    Mat left(bar, Rect(0, 0, w, bar.rows));
    cvtColor(left, left, CV_BGR2GRAY);

    imshow(QString("bar-mid").toStdString(), left);

    uchar c = Util::getMainBgGrayColor(left);
    qDebug() << "bgColor=" << c;
    bool ret = IS_ATTACK_COLOR(c);
    return ret;
}

bool WarEventRecognizer::parseBar(Mat bar)
{
    return false;
}
