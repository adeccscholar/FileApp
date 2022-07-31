//---------------------------------------------------------------------------
#include <tchar.h>
#include <minwindef.h>

#if defined BUILD_WITH_VCL || BUILD_WITH_FMX

#if defined BUILD_WITH_VCL
	#include <vcl.h>
#elif defined BUILD_WITH_FMX
	#include <fmx.h>
#endif

#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", frmMain);
//---------------------------------------------------------------------------


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
#elif defined BUILD_WITH_QT
#include <QtWidgets/QApplication>
//...QMainForm?
#else
#include <MyForm.h> //->MyFramework_Selection
#include "MainForm.h" //->TProcess

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	return 0;
}
#endif