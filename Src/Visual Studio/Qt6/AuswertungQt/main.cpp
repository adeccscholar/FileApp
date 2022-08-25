#include "AuswertungQt.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	 //open qss file
	 QFile file(":/Data/adecc_scholar.css"); // ":/Data/TCobra.qss"
	 file.open(QFile::ReadOnly);

	 QString styleSheet{ QLatin1String(file.readAll()) };
	 //setup stylesheet
	 a.setStyleSheet(styleSheet);

    AuswertungQt w;
    w.show();
    return a.exec();
}
