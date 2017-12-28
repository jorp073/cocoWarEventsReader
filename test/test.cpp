#include "test.h"

#include "recognizer/dialogrecognizer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "define.h"
#include "recognizer/wareventrecognizer.h"
#include "recognizer/wareventbarrecognizer.h"
#include "recognizer/wareventstarsrecognizer.h"
#include <QDebug>
#include "util/util.h"

using namespace cv;
using namespace std;


Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::test_dialogPageIndex()
{
    return;
    DialogRecognizer rec;

    int index;
    bool ret;

    ret = rec.recognizePageIndex(imread(RPATH "dialog/2of4.png", 1), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 1);

    ret = rec.recognizePageIndex(imread(RPATH "dialog/3of4.png", 1), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 2);
}

void Test::test_warEventBar()
{
    return;
    WarEventRecognizer rec;

    vector<Mat> bars;
    rec.findBars(imread(RPATH "dialog/2of4.png", 1), bars);

    QCOMPARE((int)bars.size(), 6);
}

void Test::test_warEventBarRemoveBg()
{
    WarEventBarRecognizer rec;
    Mat grayBar = imread(RPATH "bar/grayBar1.png", IMREAD_GRAYSCALE);
    imshow("bar", grayBar);
    Mat bgColor = Util::getHoriProjectionBgColor(grayBar);
    Util::removeHoriProjectionBg(grayBar, bgColor);
    imshow("bar remove bg", grayBar);
    waitKey();
}

void Test::test_warEventAttackDefense()
{
    return;
    WarEventRecognizer rec;
    WarEventBarRecognizer barRec;

    vector<Mat> bars;
    rec.findBars(imread(RPATH "dialog/2of4.png", 1), bars);

    vector<bool> isAttacks = {true, false, false, false, false, true};

    auto iter = isAttacks.begin();
    for (auto const & bar : bars) {
        QCOMPARE(barRec.isAttack(bar), (bool)*iter);
        iter++;
    }

    //waitKey(0);
}

void Test::test_warEventBarCutHeight()
{
    WarEventRecognizer rec;
    WarEventBarRecognizer barRec;

    vector<Mat> bars;
    rec.findBars(imread(RPATH "dialog/2of4.png", 1), bars);

    for (auto const & bar : bars) {
        Mat grayBar;
        cvtColor(bar, grayBar, CV_BGR2GRAY);

        imshow("grayBar", grayBar);
        //imwrite(RPATH "bar_gray.bmp", grayBar);

        int height = barRec.scanBarBottomHeight(grayBar);
        qDebug() << "height=" << height;

        QVERIFY( height>=51 && height <=53);
    }
}

void Test::test_warEventStars()
{
    const Rect RECTS[] = {
        Rect(501 , 0 , 106 , 41),
        Rect(510 , 0 , 97 , 40),
        Rect(501 , 0 , 111 , 40),
        Rect(502 , 0 , 105 , 40),
        Rect(510 , 0 , 97 , 40),
        Rect(508 , 0 , 99 , 40),
    };

    const int STARS[] = {
        1,
        0,
        3,
        1,
        0,
        1,
    };

    WarEventStarsRecognizer rec;
    for (size_t i=0; i<sizeof(STARS) / sizeof(int); i++) {
        const QString grayBarPath = QString(RPATH "star/grayBar%1.png").arg(i+1);
        qDebug() << "grayBarPath: " << grayBarPath;
        int stars = 0;

        const QString binBarPath = QString(RPATH "star/binBar%1.png").arg(i+1);
        bool ret = rec.recognize(imread(grayBarPath.toStdString(), IMREAD_GRAYSCALE),
                                 imread(binBarPath.toStdString(), IMREAD_GRAYSCALE),
                                 RECTS[i], &stars);
        QCOMPARE(ret, true);
        QCOMPARE(stars, STARS[i]);
    }
}

void Test::test_warEvent()
{
    WarEventRecognizer rec;

    bool ret = rec.recognize(imread(RPATH "2of4.png", 1));
    QCOMPARE(ret, true);
}
