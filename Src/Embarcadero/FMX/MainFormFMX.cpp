//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MainFormFMX.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMainFMX *frmMainFMX;
//---------------------------------------------------------------------------
__fastcall TfrmMainFMX::TfrmMainFMX(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::FormCreate(TObject *Sender)
{
   try {
      proc.Init( { this, false } );   
      }
   catch(std::exception &ex) {
      ShowMessage(ex.what());
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::btnCountClick(TObject *Sender) {
   auto get_item = [this](int iRow, int iCol) {
      return std::string(AnsiString(this->lvOutput->Cells[iCol][iRow]).c_str()); 
      };

   try {

      std::cerr << get_item(5, 0) << std::endl;

      std::vector<size_t> selected_rows;
      selected_rows.push_back(static_cast<size_t>(lvOutput->Selected));

      std::for_each(selected_rows.begin(), selected_rows.end(), [get_item](auto val) { 
           std::cerr << get_item(val, 0) << std::endl;
           });

      proc.CountAction();   
      }
   catch(std::exception &ex) {
      ShowMessage(ex.what());
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::btnShowClick(TObject *Sender)
{
   try {
      proc.ShowAction();   

      std::cerr << lvOutput->RowCount << std::endl;
      std::cerr << lvOutput->ColumnCount << std::endl;
      }
   catch(std::exception &ex) {
      ShowMessage(ex.what());
   }
   
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::btnParseClick(TObject *Sender)
{
   try {
      proc.ParseAction();   
      }
   catch(std::exception &ex) {
      ShowMessage(ex.what());
   }
}
//---------------------------------------------------------------------------
