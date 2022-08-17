//---------------------------------------------------------------------------

#ifndef MainFormFMXH
#define MainFormFMXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Rtti.hpp>
//---------------------------------------------------------------------------
#include <Process.h>
#include <FMX.Edit.hpp>

class TfrmMainFMX : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
   TPanel *Panel1;
   TPanel *Panel2;
   TButton *btnCount;
   TButton *btnShow;
   TListBox *lbValues;
   TButton *btnParse;
   TStatusBar *StatusBar1;
   TLabel *sbMain;
   TMemo *memError;
   TSplitter *Splitter1;
   TStringGrid *lvOutput;
   TPanel *Panel3;
   TLabel *lblDirectory;
   TEdit *edtDirectory;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall btnCountClick(TObject *Sender);
   void __fastcall btnShowClick(TObject *Sender);
   void __fastcall btnParseClick(TObject *Sender);
private:	// Benutzer-Deklarationen
   TProcess proc;
public:		// Benutzer-Deklarationen
   __fastcall TfrmMainFMX(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMainFMX *frmMainFMX;
//---------------------------------------------------------------------------
#endif
