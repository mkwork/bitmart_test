#include "BitmartTest.h"
#include "ui_BitmartTest.h"

#include <QFileSystemModel>
#include <QWheelEvent> //FIXME: remove
#include <QStandardItemModel>
#include <QDebug>

static const QSize MAXIMUM_ICON_SIZE = QSize(200, 200);
static const QSize MINIMUM_ICON_SIZE = QSize(50, 50);


BitmartTest::BitmartTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitmartTest),
    fileSystem (new QFileSystemModel(this))
{
    ui->setupUi(this);

    fileSystem->setRootPath(QString());
    fileSystem->setFilter(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);
    fileSystem->setReadOnly(true);



    QStandardItemModel* model = new QStandardItemModel();
    for (int i = 0; i < 20; ++i) {
        model->appendRow(new QStandardItem(QIcon("/home/maxim/skype/bmtest/1.jpg"),
                                           QString())
                         );
    }

    ui->fileSystemView->setModel(fileSystem);
    ui->fileSystemView->hideColumn(1);
    ui->fileSystemView->hideColumn(2);
    ui->fileSystemView->hideColumn(3);


    ui->imagesView->installEventFilter(this);
    ui->imagesView->setViewMode(QListView::IconMode);
    ui->imagesView->setIconSize(QSize(200, 200));
    ui->imagesView->setModel(model);

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 3);


}

BitmartTest::~BitmartTest()
{
    delete ui;
}

bool BitmartTest::eventFilter(QObject *obj, QEvent *event)
{

    if (obj != ui->imagesView)
        return false;

    if (QEvent::Wheel == event->type()) {
        processWhellScrollEvent(static_cast<QWheelEvent*>(event));
        return true;
    }


    return false;
}

void BitmartTest::processWhellScrollEvent(QWheelEvent *event)
{
    QSize size = ui->imagesView->iconSize();

    const QPoint numDegrees = event->angleDelta() / 8;
    const int stepY = numDegrees.y();
    const QSize step = QSize(stepY, stepY);
    size += step;

    if (size.height() > (MAXIMUM_ICON_SIZE).height()) {
        size = MAXIMUM_ICON_SIZE;
    } else if (size.height() < (MINIMUM_ICON_SIZE).height()){
        size = MINIMUM_ICON_SIZE;
    }

    ui->imagesView->setIconSize(size);

}
