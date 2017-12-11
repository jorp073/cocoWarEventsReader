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

bool WarEventStarsRecognizer::recognize(cv::Mat grayBar, cv::Mat binBar, cv::Rect starsRect, int* stars)
{
    int ret = 0;
    bool isLastStar = true;
    int starWidth = starsRect.width/3;
    for (int i=0; i<3; i++) {
        Rect starRect(starsRect.x + starWidth*i, starsRect.y, starWidth, starsRect.y + starsRect.height/2);
        bool isStar = isRectStar(grayBar, binBar, starRect);

        if (!isLastStar && isStar) {
            return false;
        }
        isLastStar = isStar;

        if (isStar) ret++;
    }

    *stars = ret;

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

bool WarEventStarsRecognizer::isRectStar(Mat grayBar, Mat binBar, Rect starRect)
{
    qDebug() << "binBar depth=" << binBar.depth() << "type=" << binBar.type() << "channels=" << binBar.channels();

    Mat gStar = Mat(grayBar, starRect);
    imshow("starRectGray", gStar);
    Mat bStar = Mat(binBar, starRect);
    imshow("starRectBin", bStar);
    qDebug() << "bStar depth=" << bStar.depth() << "type=" << bStar.type() << "channels=" << bStar.channels();

    int erosion_size = 4;
    int erosion_type = MORPH_ELLIPSE; // MORPH_CROSS, MORPH_ELLIPSE
    Mat element = getStructuringElement( erosion_type,
                                           Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                           Point( erosion_size, erosion_size ) );
    erode(bStar, bStar, element);
    imshow("starRectBinErode", bStar);
    waitKey(0);

    int width = bStar.cols;
    int height = bStar.rows;

    int i, j;
    const uchar *gray;
    const uchar *bin;

    int starMore = 0;
    for(i = 0; i < height; i++){
        gray = gStar.ptr<uchar>(i);
        bin = bStar.ptr<uchar>(i);
        for(j = 0; j < width; j++){
            if (bin[j] > 0) {
                if (isStarColor(gray[j])) {
                    starMore++;
                } else {
                    starMore--;
                }
            }
        }
    }

    qDebug() << "starMore=" << starMore;
    return starMore > 0;
}
