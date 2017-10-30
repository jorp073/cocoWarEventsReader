#include "wareventbarrecognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "util/util.h"
#include <QDebug>
#include "sample.h"

using namespace cv;
using namespace std;

WarEventBarRecognizer::WarEventBarRecognizer()
{

}

bool WarEventBarRecognizer::recognize(Mat bar)
{
    Mat grayBar;
    cvtColor(bar, grayBar, CV_BGR2GRAY);

    //imshow("grayBar", grayBar);

    int height = scanBarBottomHeight(grayBar);

    int dTop = height-grayBar.rows;
    bar.adjustROI(dTop, 0, 0, 0);
    grayBar.adjustROI(dTop, 0, 0, 0);
    imshow("grayBarCut", grayBar);

    Mat binBar;
    clearBackground(bar, grayBar, binBar);

    Rect flagRect1, flagRect2, attackArrowRect, playButtonRect;
    if (!filterObjects(binBar, flagRect1, flagRect2, attackArrowRect, playButtonRect)) {
        return false;
    }

    // get 3 valuable parts
    Mat ourWarrior(bar, Rect(0, 0, flagRect1.x, bar.rows));

    Mat stars;
    int flagCenterX = (flagRect1.x + flagRect2.x + flagRect2.width)/2;
    if (attackArrowRect.x < flagCenterX) {
        stars = Mat(bar, Rect(attackArrowRect.x + attackArrowRect.width,
                            0, flagRect2.x - attackArrowRect.x - attackArrowRect.width, bar.rows));
    } else {
        stars = Mat(bar, Rect(flagRect1.x + flagRect1.width,
                            0, attackArrowRect.x - flagRect1.x - flagRect1.width, bar.rows));
    }

    Mat enemyWarrior(bar, Rect(flagRect2.x + flagRect2.width, 0,
                               playButtonRect.x - flagRect2.x - flagRect2.width, bar.rows));

    imshow("ourWarrior", ourWarrior);
    imshow("stars", stars);
    imshow("enemyWarrior", enemyWarrior);

    waitKey(0);
    return true;
}

bool WarEventBarRecognizer::isAttack(Mat bar)
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

int WarEventBarRecognizer::scanBarBottomHeight(Mat grayBar)
{
    Mat bar90 = grayBar.clone();
    Util::rotateClockWise90(bar90);
    //imshow("bar90", bar90);

    int channels = bar90.channels();
    Q_ASSERT(channels == 1);
    int nRows = bar90.rows;
    int nCols = bar90.cols;

    std::vector<double> heightsCount;
    for (int j=0; j<=nCols; j++) {
        heightsCount.push_back(0);
    }

    uchar *p;
    for( int i = 0; i < nRows; ++i )
    {
        p =  bar90.ptr<uchar>(i);
        int j=0;
        for(; j < nCols; ++j )
        {
            uchar c = p[j];
            if (c != EVENTBAR_ATTACK_BGCOLOR && c != EVENTBAR_DEFENSE_BGCOLOR) {
                heightsCount[j] ++;
                break;
            }
        }
        if (j == nCols) {
            heightsCount[nCols] ++;
        }
    }

    for (int j=0; j<=nCols; j++) {
        //qDebug() << "heightsCount[" << j << "]=" << heightsCount[j];
    }

    double maxVal = 0;
    int maxIdx;
    minMaxIdx(Mat(heightsCount), NULL, &maxVal, NULL, &maxIdx);

    return maxIdx;
}

void WarEventBarRecognizer::clearBackground(Mat bar, Mat grayBar, cv::OutputArray binBar)
{
    Mat bin;
    Mat lookupTable(1, 256, CV_8U);
    uchar* p = lookupTable.data;
    for (int i = 0; i < 256; i++)
    {
        p[i] = (i == EVENTBAR_ATTACK_BGCOLOR || i == EVENTBAR_DEFENSE_BGCOLOR) ? 0 : 255;
    }

    LUT(grayBar, lookupTable, binBar);
    imshow("clearBackground", binBar);
    //imwrite(RPATH "sample.png", binBar);
}

bool WarEventBarRecognizer::filterObjects(Mat &binbar, Rect &flagRect1, Rect &flagRect2,
                                          Rect &attackArrowRect, Rect &playButtonRect)
{
    // 利用形状匹配查找部落旗帜
    // 最高的是进攻箭头
    // 最右边的是播放按钮

    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(binbar, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (contours.size() < 6) return false;

    double minMatchVal[] = {1e20, 1e20};
    int minMatchIdx[] = {0, 0};

    vector<Rect> boundingRects;

    double maxRightVal = 0;
    int maxRightIdx;

    double maxHeightVal = 0;
    int maxHeightIdx;

    for (size_t i=0; i<contours.size(); i++) {
        double match = Sample::instance().compareFlagContour(contours[i]);
        qDebug() << "match=" << match;

        if (match < minMatchVal[1]) {
            if (match < minMatchVal[0]) {
                minMatchIdx[1] = minMatchIdx[0];
                minMatchIdx[0] = i;
                minMatchVal[1] = minMatchVal[0];
                minMatchVal[0] = match;
            } else {
                minMatchIdx[1] = i;
                minMatchVal[1] = match;
            }
        }

        auto rect = boundingRect(contours[i]);
        boundingRects.push_back(rect);

        int rectRight = rect.x + rect.width;
        if (rectRight > maxRightVal) {
            maxRightVal = rectRight;
            maxRightIdx = i;
        }

        if (rect.height > maxHeightVal) {
            maxHeightVal = rect.height;
            maxHeightIdx = i;
        }
    }

    // flag rects
    flagRect1 = boundingRects[minMatchIdx[0]];
    flagRect2 = boundingRects[minMatchIdx[1]];
    if (flagRect1.x > flagRect2.x) {
        swap(flagRect1, flagRect2);
    }

    // attack arrow rect
    attackArrowRect = boundingRects[maxHeightIdx];

    // play button rect
    playButtonRect = boundingRects[maxRightIdx];

    return true;
}
