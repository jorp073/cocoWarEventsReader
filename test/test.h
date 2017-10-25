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
    void test_index();
};

#endif // TESTDIALOGRECOGNIZE_H
