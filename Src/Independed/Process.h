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
#include <thread>
#include <atomic>


class TMyTimer {
    private:
       std::atomic<bool> boActive = false;
      // std::atomic<std::vector<std::function<void ()>>> tasks;
    public:
       TMyTimer() = default;
       TMyTimer(TMyTimer const&) = delete;

       template <typename func_type, typename... arguments>
       void add_task(unsigned int interval, func_type func, arguments&&... args) {
          std::function<typename std::result_of<func_type(arguments...)>::type()> task(std::bind(std::forward<func_type>(func), std::forward<arguments>(args)...));
          //std::function<void ()> task(std::bind(std::forward<func_type>(func), std::forward<arguments>(args)...));
          std::thread([this, interval, task]() {
             while(this->boActive == true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                if(this->boActive) task();
                }
             }).detach();
          }

    void start() { boActive = true; }
    void stop() { boActive = false; }
    };



class TProcess {
   private:
      TMyForm frm;
      TMyTimer tasks;
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
