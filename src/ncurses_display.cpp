#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent, int size, bool compact) {

  float bars {percent * size};
  std::string result {""};

  if (compact == false) result += "[";
  for (int i{0}; i < size; ++i) {
    result += (bars > 0 && i <= bars) ? '|' : ' ';
  }
  if (compact == true) 
    return result;
  else
    result += "] ";

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = to_string(percent * 100).substr(0, 3);
  result += display + string("%%");
  return result;
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  
  int row{0};
  mvwprintw(window, ++row, 2, ("OS:     " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  
  vector<float> cpus = system.Cpu().Utilization();
  int i = -1;
  for (float& value : cpus) {
    if (i==-1) {

      // first item is aggregate
      mvwprintw(window, ++row, 2, "Cpu:    ");
      wattron(window, COLOR_PAIR(1));
      wprintw(window, ProgressBar(value).c_str());
      wattroff(window, COLOR_PAIR(1));

    } else {

       // remaining items are individual cores
      if (i % 4 == 0) mvwprintw(window, ++row, 2, "");
      wattron(window, COLOR_PAIR(1));
      mvwprintw(window, row, 10 + (13 * (i % 4)), "[");
      wattron(window, COLOR_PAIR(2));
      wprintw(window, to_string(i).c_str());
      wattron(window, COLOR_PAIR(1));
      wprintw(window, ProgressBar(value, 10, true).c_str());
      wprintw(window, string("]").c_str());
      wattroff(window, COLOR_PAIR(2));
    }
    i++;
  }
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes:   " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2, 
            ("Up Time:           " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {

  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));

  int end = ((int)processes.size() > n) ? n : (int)processes.size();
  for (int i = 0; i < end; ++i) {

    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {

  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(12, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    
    // draw screen
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    werase(process_window);  // previous values were leaving artifacts, this is how I solved it
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // detect keypress
    timeout(0);
    unsigned char x = getch();
    if (x == 27 || x == 'q' || x == 'Q') 
      break;   // Quit : ESC|q|Q
  }
  endwin();
}