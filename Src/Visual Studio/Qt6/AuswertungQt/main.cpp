#include "AuswertungQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuswertungQt w;
    w.show();
    return a.exec();
}
