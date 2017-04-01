#ifndef BITMARTTEST_H
#define BITMARTTEST_H

#include <QDialog>

class QFileSystemModel;
class QWheelEvent;

namespace Ui {
class BitmartTest;
}

class BitmartTest : public QDialog
{
    Q_OBJECT

public:
    explicit BitmartTest(QWidget *parent = 0);
    ~BitmartTest();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void processWhellScrollEvent(QWheelEvent* event);

    Ui::BitmartTest *ui;
    QFileSystemModel* fileSystem;
};

#endif // BITMARTTEST_H
