#include "BitmartTest.h"
#include "ui_BitmartTest.h"

BitmartTest::BitmartTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitmartTest)
{
    ui->setupUi(this);
}

BitmartTest::~BitmartTest()
{
    delete ui;
}
