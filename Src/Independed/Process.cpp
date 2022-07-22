//---------------------------------------------------------------------------

#pragma hdrstop

#include "Process.h"

#include "MyForm.h"
#include "MyStream.h"
#include "MyType_Traits.h"
#include "MyTupleUtils.h"

#include "FileUtil.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
// !!! #include <filesystem>
#include <tuple>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>
#include <exception>
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

// --------------------------------------------------------------------------
// Prozesslogik

struct TMyNum : public std::numpunct<char> {
	char_type   do_decimal_point () const { return ','; }
	char_type   do_thousands_sep () const { return '.'; }
	string_type do_truename ()      const { return "ja"; }
	string_type do_falsename ()     const { return "nein"; }
	std::string do_grouping ()      const { return "\3";	}
   };

TMyNum newNumPunct;
std::locale TProcess::myLoc;

TStreamWrapper<Narrow> old_cout(std::cout);
TStreamWrapper<Narrow> old_cerr(std::cerr);
TStreamWrapper<Narrow> old_clog(std::clog);



std::vector<tplList<Narrow>> TProcess::Project_Columns {
    		  tplList<Narrow> { "project",      360, EMyAlignmentType::left },
              tplList<Narrow> { "path",         650, EMyAlignmentType::left },
              tplList<Narrow> { "type",         150, EMyAlignmentType::left },
			  tplList<Narrow> { "order",        110, EMyAlignmentType::right },
    		  tplList<Narrow> { "cpp- file",    350, EMyAlignmentType::left },
    		  tplList<Narrow> { "rows",         110, EMyAlignmentType::right },
			  tplList<Narrow> { "h- file",      280, EMyAlignmentType::left },
			  tplList<Narrow> { "rows",         110, EMyAlignmentType::right },
			  tplList<Narrow> { "form file",   350, EMyAlignmentType::left },
			  tplList<Narrow> { "form name",    340, EMyAlignmentType::left },
			  tplList<Narrow> { "rows",         110, EMyAlignmentType::right },
			  tplList<Narrow> { "formType",     150, EMyAlignmentType::left },
			  tplList<Narrow> { "designclass",  250, EMyAlignmentType::left } };

std::vector<tplList<Narrow>> TProcess::Count_Columns {
    		  tplList<Narrow> { "files",        450, EMyAlignmentType::right },
              tplList<Narrow> { "directories",  450, EMyAlignmentType::right },
              tplList<Narrow> { "size",         600, EMyAlignmentType::right }
              };

// !!!
std::vector<tplList<Narrow>> TProcess::File_Columns {
    		  tplList<Narrow> { "file",       1310, EMyAlignmentType::left },
              tplList<Narrow> { "time",        265, EMyAlignmentType::left },
              tplList<Narrow> { "size",        150, EMyAlignmentType::right } };


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

// !!! namespace fs = std::filesystem;



//----------------------------------------------------------------------------
template <typename Iterator, typename Pred, typename Operation>
void for_each_if(Iterator begin, Iterator end, Pred pred, Operation op) {
   if(begin < end) {
      for(; begin != end; ++begin) {
         if(pred(*begin)) op(*begin);
         }
      }
   }


template <typename string_type, typename container>
size_t parse(string_type const& source, std::string const& del, container& list) {
   static_assert(is_vector<container>::value || is_set<container>::value, "unexpected value for container in function parse");
   size_t pos = 0u, end;
   do {
       end = source.find(del, pos);
       string_type part = source.substr(pos, end - pos);
       if(auto start_at = part.find_first_not_of(" \t"); start_at != string_type::npos) {
          auto end_at   = part.find_last_not_of(" \t");
          string_type trimmedpart = part.substr(start_at, end_at - start_at + 1);
          if(trimmedpart.length() > 0) {
             if constexpr (is_vector<container>::value) {
                list.emplace_back(trimmedpart);
                }
             else if constexpr (is_set<container>::value) {
                list.insert(trimmedpart);
                }
             else {
                static_assert_no_match();
                }
             }
          }
      pos = end + del.length();
      } while (end != string_type::npos);
   return list.size();
   }


// !!!
const int bits_in_byte = 8;
const int bytes_in_kilobyte = 1024;

std::uintmax_t Convert_Size_KiloByte(std::uintmax_t val) {
   return val / bytes_in_kilobyte + 1;
   }


// C++20 format for date time, C++Builder only C++17
void TProcess::ShowFiles(std::vector<fs::path> const& files) {
   std::for_each(files.begin(), files.end(), [](auto p) {
              if(fs::is_directory(p))
                 std::cout << p << std::endl;
              else {
                 auto ftime = std::filesystem::last_write_time(p);
                 auto tt = decltype(ftime)::clock::to_time_t(ftime);
                 std::tm *loctime = std::localtime(&tt);
                 std::cout << p << '\t'
                           << std::put_time(loctime, "%d.%m.%Y %T") << '\t'
                           << Convert_Size_KiloByte(fs::file_size(p)) << " KB" << std::endl;
                 }
              });
   }



 /**
   \brief Intitialize the main window of the application
   \param form [IN] rvalue to form, which is used as main window
   \throw std::runtime_error
*/
 void TProcess::Init(TMyForm&& form) {
   swap(frm, form);
   std::ios_base::sync_with_stdio(false);
   myLoc = std::locale(std::locale("de_DE"), &newNumPunct);

   frm.GetAsStream<Narrow, EMyFrameworkType::listview>(old_cout, "lvOutput", Project_Columns);
   frm.GetAsStream<Narrow, EMyFrameworkType::memo>(old_cerr, "memError");
   frm.GetAsStream<Narrow, EMyFrameworkType::statusbar>(old_clog, "sbMain");

   for(auto stream : { &std::cout, &std::cerr, &std::clog } ) {
      stream->imbue(myLoc);
      stream->setf(std::ios::fixed);
      }

   frm.SetCaption("analyzing projects");

   frm.Set<EMyFrameworkType::label>("lblDirectory", "directory:");
   frm.Set<EMyFrameworkType::edit>("edtDirectory", "d:\\projekte\\vorlesung");

   frm.Set<EMyFrameworkType::button>("btnCount", "count");
   frm.Set<EMyFrameworkType::button>("btnShow",  "show");     // !!!
   frm.Set<EMyFrameworkType::button>("btnParse", "parse");

   std::ostream mys(frm.GetAsStreamBuff<Narrow, EMyFrameworkType::listbox>("lbValues"));
   std::vector<std::string> test = { ".cpp", ".h", ".dfm", ".fmx", ".cbproj", ".c", ".hpp" };
   std::copy(test.begin(), test.end(), std::ostream_iterator<std::string>(mys, "\n"));

   std::ostream mys2(frm.GetAsStreamBuff<Narrow, EMyFrameworkType::combobox>("cbValues"));
   std::copy(test.begin(), test.end(), std::ostream_iterator<std::string>(mys2, "\n"));

   std::clog << "program ready for action" << std::endl;

   }



void TProcess::ShowAction() {
   try {
      std::vector<fs::path> files;
      //std::set<std::string> extensions = { ".cpp", ".c", ".h", ".hpp", ".hxx", ".cbproj", ".dfm", ".fmx" };
      std::set<std::string> extensions;
      my_formlist<EMyFrameworkType::listbox, std::string> mylist(&frm, "lbValues");
      std::copy(mylist.begin(), mylist.end(), std::ostream_iterator<std::string>(std::cerr, "\n"));
      std::copy(mylist.begin(), mylist.end(), std::inserter(extensions, extensions.end()));

      auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
      if(!strPath) {
         throw std::runtime_error("directory to show is empty, set a directory before call this function");
         }
      else {
         frm.GetAsStream<Narrow, EMyFrameworkType::listview>(old_cout, "lvOutput", File_Columns);
         std::chrono::milliseconds time;
         fs::path fsPath = *strPath;
         auto ret = Call(time, Find, std::ref(files), std::cref(fsPath), std::cref(extensions), true);
         std::clog << "function \"Find\" procecced in "
                   << std::setprecision(3) << time.count()/1000. << " sec" << std::endl;
         ShowFiles(files);
         }
      }
   catch(std::exception& ex) {
      std::cerr << "error in function \"Show\": " << ex.what() << std::endl;
      std::clog << "error in function \"Show\"" << std::endl;
      }
   }




void TProcess::ParseAction() {
   std::vector<fs::path> files;
   std::set<std::string> extensions = { ".cpp", ".c", ".h", ".hpp", ".hxx", ".cbproj", ".dfm", ".fmx" };

   try {
      auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
      if(!strPath) {
         throw std::runtime_error("directory to parse is empty, set a directory before call this function");
         }
      else {
         frm.GetAsStream<Narrow, EMyFrameworkType::listview>(old_cout, "lvOutput", Project_Columns);
         std::chrono::milliseconds time;
         fs::path fsPath = *strPath;
         auto ret = Call(time, Find, std::ref(files), std::cref(fsPath), std::cref(extensions), true);
         std::cerr << ret << std::endl
                   << "procecced in " << std::setprecision(3) << time.count()/1000. << " sec" << std::endl;
         }
      }
   catch(std::exception &ex) {
      std::cerr << "error in function \"Parse\": " << ex.what() << std::endl;
      std::clog << "error in function \"Parse\"" << std::endl;
      }
   }

void TProcess::CountAction() {
   try {
      auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
      if(!strPath) {
         throw std::runtime_error("directory to count is empty, set a directory before call this function");
         }
      else {
         frm.GetAsStream<Narrow, EMyFrameworkType::listview>(old_cout, "lvOutput", Count_Columns);
         std::chrono::milliseconds time;
         fs::path fsPath = *strPath;
         auto ret = Call(time, Count, std::cref(fsPath), true);
         std::get<2>(ret) = Convert_Size_KiloByte(std::get<2>(ret));
         TMyDelimiter<Narrow> delimiter = { "", "\t", "\n" };
         myTupleHlp<Narrow>::Output(std::cout, delimiter, ret);
         std::clog << "function \"Count\" procecced in "
                   << std::setprecision(3) << time.count()/1000. << " sec" << std::endl;
         }
      }
   catch(std::exception &ex) {
      std::cerr << "error in function \"Count\": " << ex.what() << std::endl;
      std::clog << "error in function \"Count\"" << std::endl;
      }
   }
