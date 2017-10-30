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

void WarEventBarRecognizer::recognize(Mat bar)
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

    clearFlags(binBar);

    waitKey(0);
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
    imshow("bar90", bar90);

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

void WarEventBarRecognizer::clearFlags(Mat& binbar)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(binbar, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    double minMatchVal[] = {1e20, 1e20};
    int minMatchIdx[] = {0, 0};

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
    }

    //将轮廓内部填充为黑色，其他区域为黑色：
    for (int i=0; i<2; i++) {
        drawContours(binbar, contours, minMatchIdx[i],  CV_RGB(0,0,0), CV_FILLED);
    }

    imshow("clearFlags", binbar);
}
