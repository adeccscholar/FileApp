#include "NukForm.h"


// Purpose: Connect Form-Events to Process.h::TProcess
//---------------------------------------------------------------------------
//here the MainForm.cpp-Variant from the VCL-Branch
#if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
// TObject/TComponent in System.Classes.hpp
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

void __fastcall TfrmMain::FormCreate(TObject *Sender) {
	proc.Init({ this, false });
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCountClick(TObject *Sender) {
	TMyForm frm(this);
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
#elif defined BUILD_WITH_NUKLEAR

#endif
