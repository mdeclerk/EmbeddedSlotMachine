#include <ncurses.h>
#include "ncursesui.h"

#define CIRCULAR_INC(val, num) val = (val + 1) % num;
#define CIRCULAR_DEC(val, num) val = (val - 1 + num) % num;

NcursesUI::NcursesUI(int width, int height, int delay) 
    : width(width)
    , height(height)
    , delay(delay) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    this->width = width > 0 ? width : getmaxx(stdscr);
    this->height = height > 0 ? height : getmaxy(stdscr);
    timeout(delay);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    //bkgd(COLOR_PAIR(1));

    clear();
    refresh();
}

NcursesUI::~NcursesUI() {
    endwin();
}

void NcursesUI::drawChar(Point2i p, char ch) const {
    mvaddch(p.y, p.x, ch);
}

void NcursesUI::drawChars(const std::vector<Point2i>& points, char ch) const {
    for (const auto& p : points)
        mvaddch(p.y, p.x, ch);
}

void NcursesUI::drawText(Point2i p, const std::string& text) const {
    mvprintw(p.y, p.x, "%s", text.c_str());
}

void NcursesUI::drawFrame(Point2i p1, Point2i p2) const {
    for (int x = p1.x + 1; x < p2.x - 1; ++x) {
        mvaddch(p1.y, x, ACS_HLINE);
        mvaddch(p2.y - 1, x, ACS_HLINE);
    }

    for (int y = p1.y + 1; y < p2.y - 1; ++y) {
        mvaddch(y, p1.x, ACS_VLINE);
        mvaddch(y, p2.x - 1, ACS_VLINE);
    }

    mvaddch(p1.y, p1.x, ACS_ULCORNER);
    mvaddch(p1.y, p2.x - 1, ACS_URCORNER);
    mvaddch(p2.y - 1, p1.x, ACS_LLCORNER);
    mvaddch(p2.y - 1, p2.x - 1, ACS_LRCORNER);
}

void NcursesUI::clear() const {
    ::clear();
}

void NcursesUI::refresh() const {
    ::refresh();
}

NcursesUI::Key NcursesUI::getKey() const {
    switch (getch()) {
        case KEY_UP:        return Key::Up;
        case KEY_RIGHT:     return Key::Right;
        case KEY_DOWN:      return Key::Down;
        case KEY_LEFT:      return Key::Left;
        case 10:     return Key::Enter;
        case ' ':           return Key::Space;
        case 'p':           return Key::P;
        case 'q':           return Key::Q;
        case 27:            return Key::Esc;
    }
    return Key::None;
}

int NcursesUI::showOptionsDialog(const std::string& title, const std::vector<std::string>& options) const {
    size_t noptions = options.size();
    int popupWidth = 30, popupHeight = 4 + noptions;
    int popupX = (width - popupWidth) / 2;
    int popupY = (height - popupHeight) / 2;

    WINDOW* win = newwin(popupHeight, popupWidth, popupY, popupX);
    wbkgd(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(1));
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "%s", title.c_str());
    for (size_t i = 0; i < noptions; ++i) {
        mvwprintw(win, 3 + i, 2, "%s %s", (i == 0) ? ">" : " ", options[i].c_str());
    }
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);

    int selection = 0;
    for (;;) {
        int ch = getch();
        switch (ch) {
            case KEY_UP:    CIRCULAR_DEC(selection, noptions); break;
            case KEY_DOWN:  CIRCULAR_INC(selection, noptions); break;
            case 10:        delwin(win); return selection;
        }

        wattron(win, COLOR_PAIR(1));
        for (size_t i = 0; i < noptions; ++i) {
            mvwprintw(win, 3 + i, 2, "%s %s", ((int)i == selection) ? ">" : " ", options[i].c_str());
        }
        wattroff(win, COLOR_PAIR(1));
        wrefresh(win);
    }
}