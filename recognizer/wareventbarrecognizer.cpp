#include "wareventbarrecognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "util/util.h"
#include <QDebug>
#include "sample.h"
#include "wareventstarsrecognizer.h"

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

    map<int, Rect> objectRects;
    if (!getSegments(binBar, objectRects)) {
        return false;
    }

    // get parts
    vector<Mat> parts;
    Rect starsRect;
    for (size_t i=0; i<objectRects.size(); i++) {
        auto rect = objectRects[i];
        parts.push_back(Mat(grayBar, rect));

        if (1 == i) {
            starsRect = rect;
        }
    }

    imshow("ourWarrior", parts[0]);
    imshow("stars", parts[1]);
    imshow("enemyWarrior", parts[2]);

    { // only for debug
        static int i=0;
        i++;
        //imwrite(RPATH "bar/input.png", bar);
        //imwrite(RPATH "bar/output_stars_part.png", parts[1]);
        //imwrite(QString(RPATH "star/grayBar%1.png").arg(i).toStdString().c_str(), grayBar);
        //imwrite(QString(RPATH "star/binBar%1.png").arg(i).toStdString().c_str(), binBar);
        qDebug() << "starsRect" << i << "=" << starsRect.x << ',' << starsRect.y << ',' << starsRect.width << ',' << starsRect.height;
    }

    WarEventStarsRecognizer starsRec;
    int stars;
    if (!starsRec.recognize(grayBar, binBar, starsRect, &stars)) {
        return false;
    }

    qDebug() << "stars=" << stars;

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

bool WarEventBarRecognizer::getSegments(Mat &binBar,  std::map<int, cv::Rect> &objectRects)
{
    // 利用形状匹配查找部落旗帜
    // 最高的是进攻箭头
    // 最右边的是播放按钮

    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(binBar, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (contours.size() < 6) {
        return false;
    }

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

    /// 4 splitter rects
    vector<Rect> splitters;

    // flag rects
    Rect flagRect1 = boundingRects[minMatchIdx[0]];
    Rect flagRect2 = boundingRects[minMatchIdx[1]];
    if (flagRect1.x > flagRect2.x) {
        swap(flagRect1, flagRect2);
    }
    splitters.push_back(flagRect1);

    // attack arrow rect
    Rect attackArrowRect = boundingRects[maxHeightIdx];
    splitters.push_back(attackArrowRect);

    splitters.push_back(flagRect2);

    // play button rect
    Rect playButtonRect = boundingRects[maxRightIdx];
    splitters.push_back(playButtonRect);

    for (size_t i=0; i<splitters.size()-1; i++) {
        if (splitters[i].x + splitters[i].width >= splitters[i+1].x) {
            // splitter overlapped
            return false;
        }
    }

    /// 3 parts
    vector<Point2i> parts;

    // our warrior rect
    parts.push_back(Point2i(0, flagRect1.x));

    // stars rect
    int flagCenterX = (flagRect1.x + flagRect2.x + flagRect2.width)/2;
    if (attackArrowRect.x < flagCenterX) {
        parts.push_back(Point2i(attackArrowRect.x + attackArrowRect.width, flagRect2.x));
    } else {
        parts.push_back(Point2i(flagRect1.x + flagRect1.width, attackArrowRect.x));
    }

    // enemy warrior rect
    parts.push_back(Point2i(flagRect2.x + flagRect2.width, playButtonRect.x));

    /// zip segments
    for (size_t i=0; i<parts.size(); i++) {
        objectRects[i] = Rect(binBar.cols + 1, binBar.rows + 1, -binBar.cols-2, -binBar.rows-2);
        Q_ASSERT(objectRects[i].br().x < 0);
        Q_ASSERT(objectRects[i].br().y < 0);
    }

    for (auto const & boundingRect : boundingRects) {
        for (size_t i=0; i<parts.size(); i++) {
            bool inside = boundingRect.x > parts[i].x && boundingRect.br().x < parts[i].y;
            if (inside) {
                auto & rect = objectRects[i];
                if (boundingRect.x < rect.x) {
                    rect.width += rect.x - boundingRect.x;
                    rect.x = boundingRect.x;
                }
                if (boundingRect.y < rect.y) {
                    rect.height += rect.y - boundingRect.y;
                    rect.y = boundingRect.y;
                }
                if (boundingRect.br().x > rect.br().x) {
                    rect.width = boundingRect.br().x - rect.x;
                }
                if (boundingRect.br().y > rect.br().y) {
                    rect.height = boundingRect.br().y - rect.y;
                }
            }
        }
    }

    for (size_t i=0; i<objectRects.size(); i++) {
        auto rect = objectRects[i];
        if (rect.x<0 || rect.y<0 || rect.br().x >= binBar.cols || rect.br().y >= binBar.rows) {
            // the objectRect has no objects;
            return false;
        }
    }

    return true;
}
