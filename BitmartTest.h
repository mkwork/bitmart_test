#ifndef BITMARTTEST_H
#define BITMARTTEST_H

#include <QDialog>

class QFileSystemModel;
class QWheelEvent;
class QStandardItemModel;

namespace Ui {
class BitmartTest;
}

class BitmartTest : public QDialog
{
    Q_OBJECT

public:
    explicit BitmartTest(QWidget *parent = 0);
    ~BitmartTest();

protected slots:
    void onDirectoryLoaded(const QString& path);
    void onDirectoryChanged(const QModelIndex& current,
                            const QModelIndex& previous);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void processWhellScrollEvent(QWheelEvent* event);

    Ui::BitmartTest *ui;
    QFileSystemModel* fileSystem;
    QStandardItemModel* images;
};

#endif // BITMARTTEST_H
