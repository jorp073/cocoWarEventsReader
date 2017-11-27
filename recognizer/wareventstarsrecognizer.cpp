#include "wareventstarsrecognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "util/util.h"
#include <QDebug>

using namespace std;
using namespace cv;

WarEventStarsRecognizer::WarEventStarsRecognizer()
{

}

bool WarEventStarsRecognizer::recognize(cv::Mat grayImg, int* stars)
{
    vector<float> rxs = {0.18f, 0.5f, 0.82f};
    auto p = grayImg.ptr<uchar>(0);
    Q_ASSERT(p != NULL);
    vector<uchar> colors;

    auto pushColor = [&](float rx) {
        int index = grayImg.cols * rx;
        colors.push_back(p[index]);
    };

    for_each(rxs.begin(), rxs.end(), pushColor);

    vector<bool> isStars;
    *stars = 0;
    for (auto iter = colors.begin(); iter != colors.end(); ++iter) {
        qDebug() << "starColor=" << *iter;
        bool isStar = isStarColor(*iter);
        isStars.push_back(isStar);
        if (isStar) (*stars)++;
    }

    if ((!isStars[1] && isStars[2]) || (!isStars[0] && isStars[1])) {
        return false;
    }

    return true;
}

bool WarEventStarsRecognizer::isStarColor(uchar color)
{
    auto const starColors = STAR_COLORS;

    double minVal = 1e20;
    STAR_TYPE minIdx = starColors.begin()->first;
    for (auto iter = starColors.begin(); iter != starColors.end(); ++iter) {
        double v = iter->second - color;
        double dist = v*v;
        if (dist < minVal) {
            minVal = dist;
            minIdx = iter->first;
        }
    }

    return minIdx == HIGHTLIGHT || minIdx == STAR;
}
