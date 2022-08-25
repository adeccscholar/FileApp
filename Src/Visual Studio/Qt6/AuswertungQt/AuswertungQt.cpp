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


      std::cerr << ui.lvOutput->rowCount() << std::endl;
      std::cerr << ui.lvOutput->columnCount() << std::endl;
      //std::cerr << get_item(5, 0) << std::endl;
   }
   catch (std::exception& ex) {
      QMessageBox msg;
      msg.setText(ex.what());
      msg.exec();
   }
}

void AuswertungQt::Count() {
   auto get_item = [this](int iRow, int iCol) {
      return this->ui.lvOutput->item(iRow, iCol)->text().toStdString();
   };

   try {

      std::vector<size_t> rows(ui.lvOutput->rowCount());
      std::generate(rows.begin(), rows.end(), [i = 0]() mutable { return i++; });
      //std::for_each(rows.begin(), rows.end(), [get_item](auto val) { std::cerr << get_item(val, 0) << std::endl; });

      /*
      QList<QTableWidgetItem*> selected = ui.lvOutput->selectedItems();
      if(!selected.isEmpty()) {
         std::set<size_t> sel;
         for (auto item : selected) sel.insert(item->row());
         std::for_each(sel.begin(), sel.end(), [get_item](auto val) { std::cerr << get_item(val, 0) << std::endl; });
         }
      */
      QItemSelectionModel* selectModel = ui.lvOutput->selectionModel();
      foreach(QModelIndex index, selectModel->selectedRows()) std::cerr << get_item(index.row(), 0) << std::endl;

      proc.CountAction();
   }
   catch (std::exception& ex) {
      QMessageBox msg;
      msg.setText(ex.what());
      msg.exec();
   }
}

