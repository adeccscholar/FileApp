//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner) {
   }

void __fastcall TfrmMain::FormCreate(   TObject *Sender) {
   proc.Init( { this, false });
   }
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCountClick(TObject *Sender) {
   auto get_item = [this](int iRow, int iCol) {
      // range check
      TListItem* item = this->lvOutput->Items->Item[iRow];
      if(iCol == 0) {
         return std::string(AnsiString(item->Caption).c_str());
         }
      else {
         return std::string(AnsiString(item->SubItems->Strings[iCol - 1]).c_str()); 
         }
      };

   //std::cerr << get_item(5, 0) << std::endl;

   std::vector<size_t> rows(lvOutput->Items->Count);
   std::generate(rows.begin(), rows.end(), [i = 0]() mutable { return i++; });

   //std::for_each(rows.begin(), rows.end(), [get_item](auto val) { std::cerr << get_item(val, 0) << std::endl; });

   std::vector<size_t> selected_rows;
   TItemStates selected =TItemStates() << isSelected;
   for(TListItem* item = lvOutput->Selected; item; item = lvOutput->GetNextItem(item, sdAll, selected)) {
      selected_rows.push_back(static_cast<size_t>(item->Index));
      }
   
   std::for_each(selected_rows.begin(), selected_rows.end(), [get_item](auto val) { std::cerr << get_item(val, 0) << std::endl; });

   proc.CountAction();
   
   }
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnParseClick(TObject *Sender) {
   proc.ParseAction();
   }
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnShowClick(TObject *Sender) {
   proc.ShowAction();

   std::cerr << lvOutput->Items->Count << std::endl;
   std::cerr << lvOutput->Columns->Count << std::endl;
   }
//---------------------------------------------------------------------------




