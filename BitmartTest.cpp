#include "BitmartTest.h"
#include "ui_BitmartTest.h"
#include "ImagesLoader.h"

#include <QFileSystemModel>
#include <QWheelEvent>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QThreadPool>

#include <QDebug>

static const QSize MAXIMUM_ICON_SIZE = QSize(200, 200);
static const QSize MINIMUM_ICON_SIZE = QSize(50, 50);


BitmartTest::BitmartTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitmartTest),
    fileSystem (new QFileSystemModel(this))
{
    ui->setupUi(this);

    fileSystem->setRootPath(QString("/home/maxim/skype"));
    fileSystem->setFilter(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);
    fileSystem->setReadOnly(true);




    images = new QStandardItemModel();

    ui->fileSystemView->setModel(fileSystem);
    ui->fileSystemView->hideColumn(1);
    ui->fileSystemView->hideColumn(2);
    ui->fileSystemView->hideColumn(3);


    ui->imagesView->installEventFilter(this);
    ui->imagesView->setViewMode(QListView::IconMode);
    ui->imagesView->setIconSize(MAXIMUM_ICON_SIZE);
    ui->imagesView->setUniformItemSizes(true);
    ui->imagesView->setSpacing(10);
    ui->imagesView->setModel(images);

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 3);

    loader = new ImagesLoader(this);
    QThreadPool::globalInstance()->start(loader);

    connect(ui->fileSystemView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onDirectoryChanged(QModelIndex,QModelIndex)));

    connect(loader, SIGNAL(count(int)), this, SLOT(onLoadingStart(int)),
            Qt::QueuedConnection);

    connect(loader, SIGNAL(finished()), this, SLOT(onLoadingFinish()),
            Qt::QueuedConnection);

    connect(loader, SIGNAL(ready(QPixmap)), this,
            SLOT(onImageReady(QPixmap)), Qt::QueuedConnection);

    setLoadingVisible(false);

}

BitmartTest::~BitmartTest()
{
    delete ui;
    loader->stop();
    QThreadPool::globalInstance()->waitForDone();
}

void BitmartTest::onDirectoryLoaded(const QString &path)
{
    qDebug() << path;
    loader->cancel();
    images->clear();
    loader->load(path);

}

void BitmartTest::onDirectoryChanged(const QModelIndex& current, const QModelIndex& previous)
{
    const QString currentPath = fileSystem->filePath(current);
    if (currentPath == fileSystem->filePath(previous)) {
        return;
    }

    onDirectoryLoaded(currentPath);
}

void BitmartTest::setLoadingVisible(bool show)
{
    ui->loadingLabel->setVisible(show);
    ui->progressBar->setVisible(show);
}

void BitmartTest::onLoadingStart(int count)
{
    if (count > 0) {
        setLoadingVisible(true);
        ui->progressBar->setMaximum(count);
    }
}

void BitmartTest::onLoadingFinish()
{
    setLoadingVisible(false);
}

void BitmartTest::onImageReady(const QPixmap &pixmap)
{
    QStandardItem* item = new QStandardItem(QIcon(pixmap), QString());
    images->appendRow(item);
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

bool BitmartTest::eventFilter(QObject* obj, QEvent* event)
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
