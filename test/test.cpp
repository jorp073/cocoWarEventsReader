#include "test.h"

#include "recognizer/dialogrecognizer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "define.h"

using namespace cv;



Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::test_index()
{
    DialogRecognizer rec;

    int index;
    bool ret;

    ret = rec.recognizePageIndex(imread(RPATH "2of4.png", CV_8UC4), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 1);

    ret = rec.recognizePageIndex(imread(RPATH "3of4.png", CV_8UC4), 4, &index);
    QCOMPARE(ret, true);
    QCOMPARE(index, 2);
}
