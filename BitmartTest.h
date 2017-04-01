#ifndef BITMARTTEST_H
#define BITMARTTEST_H

#include <QDialog>

class QFileSystemModel;

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
    QFileSystemModel* fileSystem;
};

#endif // BITMARTTEST_H
