#include "AuswertungQt.h"
#include <QMessagebox>

AuswertungQt::AuswertungQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.statusBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("sbMain");
    statusLabel->setText("sbMain");
    ui.statusBar->addPermanentWidget(statusLabel);

    connect(ui.btnCount, SIGNAL(clicked()), this, SLOT(Count()));
    connect(ui.btnShow, SIGNAL(clicked()), this, SLOT(Show()));
    connect(ui.btnParse, SIGNAL(clicked()), this, SLOT(Parse()));

    try {
       proc.Init({ this, false });
    }
    catch (std::exception& ex) {
       QMessageBox msg;
       msg.setText(ex.what());
       msg.exec();
    }

}

AuswertungQt::~AuswertungQt(){ 
   // delete statusLabel;
}


void AuswertungQt::Parse() {
   try {
      proc.ParseAction();
   }
   catch (std::exception& ex) {
      QMessageBox msg;
      msg.setText(ex.what());
      msg.exec();
   }
}

void AuswertungQt::Show() {
   try {
      proc.ShowAction();
   }
   catch (std::exception& ex) {
      QMessageBox msg;
      msg.setText(ex.what());
      msg.exec();
   }
}

void AuswertungQt::Count() {
   try {
      proc.CountAction();
   }
   catch (std::exception& ex) {
      QMessageBox msg;
      msg.setText(ex.what());
      msg.exec();
   }
}
