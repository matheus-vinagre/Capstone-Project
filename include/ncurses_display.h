#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 50);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes,std::vector<PrevProcess>* prevProcesses, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif