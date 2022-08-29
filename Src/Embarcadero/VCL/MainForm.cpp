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
   proc.CountAction();
   }
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnParseClick(TObject *Sender) {
   proc.ParseAction();
   }
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnShowClick(TObject *Sender) {
   proc.ShowAction();
   }
//---------------------------------------------------------------------------







