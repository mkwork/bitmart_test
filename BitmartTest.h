#ifndef BITMARTTEST_H
#define BITMARTTEST_H

#include <QDialog>

class QFileSystemModel;
class QWheelEvent;
class QStandardItemModel;
class ImagesLoader;

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
    void setLoadingVisible(bool show);
    void onLoadingStart(int count);
    void onLoadingFinish();
    void onImageReady(const QPixmap& pixmap);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void processWhellScrollEvent(QWheelEvent* event);

    Ui::BitmartTest *ui;
    QFileSystemModel* fileSystem;
    QStandardItemModel* images;
    ImagesLoader* loader;
};

#endif // BITMARTTEST_H
