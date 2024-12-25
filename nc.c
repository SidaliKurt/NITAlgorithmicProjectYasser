#include <ncurses.h>

int main() {
    initscr();                  // Start ncurses mode
    start_color();              // Enable color
    init_pair(1, COLOR_RED, COLOR_BLACK);

    attron(COLOR_PAIR(1));      // Use red text
    mvprintw(10, 10, "Hello, World!");
    attroff(COLOR_PAIR(1));

    getch();                    // Wait for user input
    endwin();                   // End ncurses mode
    return 0;
}
