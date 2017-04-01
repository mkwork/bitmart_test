#ifndef BITMARTTEST_H
#define BITMARTTEST_H

#include <QDialog>

namespace Ui {
class BitmartTest;
}

class BitmartTest : public QDialog
{
    Q_OBJECT

public:
    explicit BitmartTest(QWidget *parent = 0);
    ~BitmartTest();

private:
    Ui::BitmartTest *ui;
};

#endif // BITMARTTEST_H
