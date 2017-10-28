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

void Test::test_pageIndex()
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
    WarEventRecognizer rec;

    std::vector<Mat> bars;
    rec.findWarEventBars(imread(RPATH "2of4.png", 1), bars);

    QCOMPARE((int)bars.size(), 6);
}
