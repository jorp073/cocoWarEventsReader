#include "dialogrecognizer.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <QDebug>
#include "util/util.h"


using namespace cv;

DialogRecognizer::DialogRecognizer()
{

}

bool DialogRecognizer::recognizePageIndex(cv::Mat img, int pageCount, OUT int* index)
{
    *index = 0;

    // find close button
    Rect btnRect;
    if (!findCloseButton(img, btnRect))
    {
        return false;
    }

    // cut tabs
    Rect tabbarRect(img.cols - btnRect.tl().x, btnRect.tl().y, 2* btnRect.tl().x - img.cols, btnRect.height);
    Mat tabbar(img, tabbarRect);

    cvtColor(tabbar, tabbar, CV_BGR2GRAY);

    imshow("tabbar", tabbar);

    // split tabs
    std::vector<Mat> tabs;
    int x = 0, dx = ((float)tabbar.cols)/pageCount + 0.5f;
    for (int i=0; i<pageCount-1; i++) {
        tabs.push_back(Mat(tabbar, Rect(x, 0, dx, tabbar.rows)));
        x += dx;
    }
    tabs.push_back(Mat(tabbar, Rect(x, 0, tabbar.cols-x, tabbar.rows)));

    std::vector<double> bgColors;
    for (int i=0; i<pageCount; i++) {
        imshow("tab" + QString('0' + i).toStdString(), tabs[i]);
        bgColors.push_back(Util::getMainBgGrayColor(tabs[i]));
    }

    Util::maxDistIdx(bgColors, index);

    //waitKey(0);

    return true;
}

bool DialogRecognizer::findCloseButton(cv::Mat img, OUT cv::Rect& rect)
{
    imshow("findCloseButton", img);
    //waitKey(0);

    // split, get h of hsv
    Mat hsv;
    cvtColor(img, hsv, CV_BGR2HSV_FULL);

    std::vector<Mat> channels;
    split(hsv, channels);
    Mat h = channels[0];

    imshow("h", h);
    //waitKey(0);
    //imshow("s", channels[1]);
    //waitKey(0);
    //imshow("v", channels[2]);

    // binary, filter close button
    Mat closeButtonBin;
    threshold(h, closeButtonBin, 249, 255, THRESH_BINARY);
    imshow("closeButtonBin", closeButtonBin);

    int erosion_size = 1;
    int erosion_type = MORPH_RECT; // MORPH_CROSS, MORPH_ELLIPSE
    Mat element = getStructuringElement( erosion_type,
                                           Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                           Point( erosion_size, erosion_size ) );
    erode(closeButtonBin, closeButtonBin, element );
    dilate(closeButtonBin, closeButtonBin, element);

    imshow("closeButtonBin erode dilate", closeButtonBin);

    // find contours
    if (!_findCloseButtonRect(closeButtonBin, rect)) {
        return false;
    }

    //waitKey(0);

    return true;
}

bool DialogRecognizer::_findCloseButtonRect(cv::Mat mat, OUT cv::Rect& rect)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    /// 寻找轮廓
    findContours(mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    /// 最大面积的矩形
    int iMaxAreaID = -1;
    double dMaxArea = -1;
    for (size_t i = 0; i < contours.size(); i++)
    {
        auto area = fabs(contourArea(contours[i]));
        if (area < CLOSEBUTTON_MIN_AREA) continue;
        qDebug() << "canvas area: " << area;

        if (area > dMaxArea)
        {
            dMaxArea = area;
            iMaxAreaID = i;
        }
    }

    if (-1 == iMaxAreaID) // 没有找到面积足够的矩形
    {
        return false;
    }

    /// 矩形边界框
    auto boundRect = cv::boundingRect(cv::Mat(contours[iMaxAreaID]));
    rect = boundRect;
    qDebug() << "rect " << rect.tl().x << "," << rect.tl().y << " "  << rect.br().x << "," << rect.br().y;
    return true;
}

