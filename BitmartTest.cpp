#include "BitmartTest.h"
#include "ui_BitmartTest.h"

#include <QFileSystemModel>
#include <QStandardItemModel>


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


    ui->imagesView->setModel(model);
    ui->imagesView->setViewMode(QListView::IconMode);
    ui->imagesView->setIconSize(QSize(200, 200));

}

BitmartTest::~BitmartTest()
{
    delete ui;
}
