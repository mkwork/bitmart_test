#include "BitmartTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BitmartTest w;
    w.show();

    return a.exec();
}
