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
#include <FileUtil.h>
#include <locale>
#include <vector>
#include <thread>
#include <atomic>

/**
  \brief tuple with all Data for projects in cbproj- files
  \details list of parts
<table><tr><th> ID <th> Constant <th> Type <th> Description</tr>
<tr><td align="right"><b>  0 </b> <td align="left"> iMyData_Project  <td>std::string <td>name of the project file </tr>
<tr><td align="right"><b>  1 </b> <td align="left"> iMyData_Path     <td>std::string <td>path to the project file </tr>
<tr><td align="right"><b>  2 </b> <td align="left"> iMyData_Type     <td>std::string <td>type of node </tr>
<tr><td align="right"><b>  3 </b> <td align="left"> iMyData_Order    <td>int         <td>build order </tr>
<tr><td align="right"><b>  4 </b> <td align="left"> iMyData_CppFile  <td>std::string <td>source file with relative path </tr>
<tr><td align="right"><b>  5 </b> <td align="left"> iMyData_CppRows  <td>size_t      <td>rows in source file </tr>
<tr><td align="right"><b>  6 </b> <td align="left"> iMyData_H_File   <td>std::string <td>name of header file </tr>
<tr><td align="right"><b>  7 </b> <td align="left"> iMyData_H_Rows   <td>size_t      <td>rows in header file </tr>
<tr><td align="right"><b>  8 </b> <td align="left"> iMyData_FrmFile  <td>std::string <td>form file </tr>
<tr><td align="right"><b>  9 </b> <td align="left"> iMyData_FrmName  <td>std::string <td>name of the formular </tr>
<tr><td align="right"><b> 10 </b> <td align="left"> iMyData_FrmRows  <td>size_t      <td>rows in form file </tr>
<tr><td align="right"><b> 11 </b> <td align="left"> iMyData_FrmType  <td>std::string <td>formtype </tr>
<tr><td align="right"><b> 12 </b> <td align="left"> iMyData_FrmClass <td>std::string <td>design class </tr> </table>
*/
using tplData = std::tuple<std::string,  //  0 project
                           std::string,  //  1 path
                           std::string,  //  2 type
                           int,          //  3 build order
                           std::string,  //  4 cpp file
                           size_t,       //  5 rows
                           std::string,  //  6 h file
                           size_t,       //  7 rows
                           std::string,  //  8 form file
                           std::string,  //  9 form name
                           size_t,       // 10 rows
                           std::string,  // 11 formtype
                           std::string>; // 12 design class

enum class EShowVariante : int { empty, Count, Files, Projects };

class TProcess {
   private:
      TMyForm frm;
      bool boActive = false;
      EShowVariante showMode = EShowVariante::empty;
       static std::locale myLoc;
      static std::vector<tplList<Latin>> Project_Columns;
      static std::vector<tplList<Latin>> Count_Columns;
      static std::vector<tplList<Latin>> File_Columns;

      static std::set<std::string> project_extensions;
      static std::set<std::string> header_files;
      static std::set<std::string> form_files;

   public:
      void Init(TMyForm&& frm);
      void ShowAction();
      void ParseAction();
      void CountAction();

   private:
     void ParseDirectory(void);
     void Parse(fs::path const& fsPath, std::vector<fs::path>& project_files, std::vector<tplData>& projects);
     void ParseProject(fs::path const& base, fs::path const& strFile, std::vector<tplData>& projects);
     void ShowFiles(void);
     void ShowFiles(std::ostream& out, fs::path const& strBase, std::vector<fs::path> const& files);
     void CountFiles(void);


};

#endif
