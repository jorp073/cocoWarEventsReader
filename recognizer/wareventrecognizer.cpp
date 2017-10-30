#include "wareventrecognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "util/util.h"
#include <QDebug>
#include "wareventbarrecognizer.h"

using namespace cv;
using namespace std;

WarEventRecognizer::WarEventRecognizer()
{

}

bool WarEventRecognizer::recognize(cv::Mat dialog)
{
    std::vector<Mat> bars;
    findBars(dialog, bars);

    WarEventBarRecognizer barRec;

    for (auto & bar : bars) {
        // if (!isAttack(bar)) continue;
        barRec.recognize(bar);
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
