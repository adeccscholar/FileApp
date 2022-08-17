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
void __fastcall TfrmMainFMX::btnCountClick(TObject *Sender)
{
   try {
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
