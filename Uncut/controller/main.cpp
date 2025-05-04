#include "view/Uncut.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Uncut w;
    w.show();
    return a.exec();
}
