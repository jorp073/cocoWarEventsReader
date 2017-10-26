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

    QCOMPARE(rec.recognize(imread(RPATH "2of4.png", CV_8UC3), 4), 2);
    QCOMPARE(rec.recognize(imread(RPATH "3of4.png", CV_8UC3), 4), 3);
}
