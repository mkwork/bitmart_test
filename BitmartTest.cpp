#include "BitmartTest.h"
#include "ui_BitmartTest.h"

#include <QFileSystemModel>
#include <QWheelEvent>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include <QDebug>

static const QSize MAXIMUM_ICON_SIZE = QSize(200, 200);
static const QSize MINIMUM_ICON_SIZE = QSize(50, 50);
static const char* FILTERS="*.png;*.jpg;*.jpeg";


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

    connect(ui->fileSystemView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onDirectoryChanged(QModelIndex,QModelIndex)));

    setLoadingVisible(false);

}

BitmartTest::~BitmartTest()
{
    delete ui;
}

void BitmartTest::onDirectoryLoaded(const QString &path)
{
    qDebug() << path;
    images->clear();
    QDir dir(path);
    QStringList filters = QString(FILTERS).split(';');
    dir.setNameFilters(filters);
    QStringList entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    qDebug() << entries;

    foreach (QString entry , entries) {
        QStandardItem* item = new QStandardItem(QIcon(path + "/" + entry),
                                                QString());
        item->setTextAlignment(Qt::AlignCenter);
        images->appendRow(item);
    }
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
