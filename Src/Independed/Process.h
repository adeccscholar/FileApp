/**
 \file
 \brief   file with the definition of the independent part of the process logic for the FileApp program
 \details In this file the interface for the implementation of the processes is defined.
          The core is the class TProcess. By the use of the header-only library adecc Scholar
          the independence of the respective framework is achieved.
 <hr>
 \author Volker Hillmann (adecc Systemhaus GmbH)
 \date 22.07.2022 Create repository with this file
 \version 0.1
 \since Version 0.1
*/

#ifndef ProcessH
#define ProcessH
//---------------------------------------------------------------------------

#include <MyForm.h>
#include "FileUtil.h"
#include <locale>


class TProcess {
   private:
      TMyForm frm;
      static std::locale myLoc;
      static std::vector<tplList<Narrow>> Project_Columns;
      static std::vector<tplList<Narrow>> Count_Columns;
      static std::vector<tplList<Narrow>> File_Columns;
   public:
      void Init(TMyForm&& frm);
      void ShowAction();
      void ParseAction();
      void CountAction();

};

#endif
