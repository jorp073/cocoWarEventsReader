#include "test.h"

#include "recognizer/dialogrecognizer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "define.h"
#include "recognizer/wareventrecognizer.h"
#include "recognizer/wareventbarrecognizer.h"
#include "recognizer/wareventstarsrecognizer.h"
#include <QDebug>

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

    ret = rec.recognizePageIndex(imread(RPATH "2of4.png", 1), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 1);

    ret = rec.recognizePageIndex(imread(RPATH "3of4.png", 1), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 2);
}

void Test::test_warEventBar()
{
    return;
    WarEventRecognizer rec;

    vector<Mat> bars;
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

    QCOMPARE((int)bars.size(), 6);
}

void Test::test_warEventAttackDefense()
{
    return;
    WarEventRecognizer rec;
    WarEventBarRecognizer barRec;

    vector<Mat> bars;
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

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
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

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
    const map<QString, int> starsSample = {
        {"stars_0.png", 0},
        {"stars_1.png", 1},
        {"stars_1_2.png", 1},
        {"stars_1_3.png", 1},
        {"stars_3.png", 3},
    };

    WarEventStarsRecognizer rec;
    for (auto iter = starsSample.begin(); iter != starsSample.end(); ++iter) {
        const QString path = RPATH + iter->first;
        qDebug() << "stars: " << path;
        int stars = 0;
        bool ret = rec.recognize(imread(path.toStdString(), 1), &stars);
        QCOMPARE(ret, true);
        QCOMPARE(stars, iter->second);
    }
}

void Test::test_warEvent()
{
    WarEventRecognizer rec;

    bool ret = rec.recognize(imread(RPATH "2of4.png", 1));
    QCOMPARE(ret, true);
}
