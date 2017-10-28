#include "test.h"

#include "recognizer/dialogrecognizer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "define.h"
#include "recognizer/wareventrecognizer.h"

using namespace cv;



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

    std::vector<Mat> bars;
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

    QCOMPARE((int)bars.size(), 6);
}

void Test::test_warEventAttackDefense()
{
    return;
    WarEventRecognizer rec;

    std::vector<Mat> bars;
    rec.findBars(imread(RPATH "2of4.png", 1), bars);

    std::vector<bool> isAttacks = {true, false, false, false, false, true};

    auto iter = isAttacks.begin();
    for (auto const & bar : bars) {
        QCOMPARE(rec.isAttack(bar), (bool)*iter);
        iter++;
    }

    //waitKey(0);
}

void Test::test_warEvent()
{
    WarEventRecognizer rec;

    bool ret = rec.getData(imread(RPATH "2of4.png", 1));
    QCOMPARE(ret, true);
}
