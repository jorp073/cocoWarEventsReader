#ifndef TESTDIALOGRECOGNIZE_H
#define TESTDIALOGRECOGNIZE_H
#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);

signals:

private slots:
    void test_dialogPageIndex();
    void test_warEventBar();
    void test_warEventBarRemoveBg();
    void test_warEventAttackDefense();
    void test_warEventBarCutHeight();
    void test_warEventStars();
    void test_warEvent();
};

#endif // TESTDIALOGRECOGNIZE_H
