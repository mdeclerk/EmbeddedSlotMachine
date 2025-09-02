#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cerrno>

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <linux/vt.h>
#include <termios.h>
#include <fcntl.h>

#include "ncursesui.h"

namespace fs = std::filesystem;

extern "C" char **environ;

void take_vt1_text_mode() {
    int vt = open("/dev/tty0", O_RDWR | O_CLOEXEC);
    if (vt < 0) _exit(1);

    setsid();
    ioctl(vt, TIOCSCTTY, 0);
    ioctl(vt, VT_ACTIVATE, 1);
    ioctl(vt, VT_WAITACTIVE, 1);
    //ioctl(vt, KDSETMODE, KD_TEXT);

    termios t{};
    tcgetattr(vt, &t);
    tcsetattr(vt, TCSANOW, &t);

    dup2(vt, STDIN_FILENO);
    dup2(vt, STDOUT_FILENO);
    dup2(vt, STDERR_FILENO);
    if (vt > STDERR_FILENO) close(vt);
}

bool is_executable(const fs::directory_entry& entry) {
    if (!entry.is_regular_file()) return false;
    auto perms = entry.status().permissions();
    return (perms & fs::perms::owner_exec) != fs::perms::none;
}

std::vector<std::pair<std::string, std::string>> get_executables(const std::string& filepath) {
    fs::path self_path = fs::weakly_canonical(fs::absolute(filepath));
    fs::path self_dir = self_path.parent_path();
    std::string own_file = self_path.filename().string();

    std::vector<std::pair<std::string, std::string>> executables;
    for (const auto& entry : fs::directory_iterator(self_dir)) {
        if (!is_executable(entry)) continue;
        std::string fname = entry.path().filename().string();
        if (fname == own_file) continue; // Exclude self
        if (entry.path().extension() == ".so") continue; // Exclude .so files
        executables.emplace_back(fname, entry.path().string());
    }
    return executables;
}

int run_program(const std::string& exe, const std::vector<std::string>& args = {}) {
    pid_t pid = ::fork();
    if (pid == 0) { // child
        std::vector<char*> argv;
        argv.reserve(args.size() + 2);
        argv.push_back(const_cast<char*>(exe.c_str()));
        for (const auto& s : args) 
            argv.push_back(const_cast<char*>(s.c_str()));
        argv.push_back(nullptr);

        ::execve(exe.c_str(), argv.data(), environ);
        _exit(127);
    } else if (pid > 0) { // parent
        int status = 0;
        for (;;) {
            if (::waitpid(pid, &status, 0) == -1) {
                if (errno == EINTR) continue;
                return -1;
            }
            break;
        }
    
        if (WIFEXITED(status)) 
            return WEXITSTATUS(status);
        if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
    }
    return -1;
}

int main(int /*argc*/, char* argv[]) {
    if (getpid() == 1) {
        mount("devtmpfs", "/dev", "devtmpfs", MS_NOSUID|MS_NOEXEC, "mode=0755");

        setenv("TERM", "linux", 1);
        take_vt1_text_mode();

        //struct winsize ws{.ws_row=25, .ws_col=80};
        //ioctl(STDOUT_FILENO, TIOCSWINSZ, &ws);
        //setenv("LINES", std::to_string(ws.ws_row).c_str(), 1);
        //setenv("COLUMNS", std::to_string(ws.ws_col).c_str(), 1);
    }

    auto executables = get_executables(argv[0]);
    if (executables.empty()) {
        std::cerr << "no executables found in current directory\n";
        return -1;
    }

    std::vector<std::string> menu;
    menu.reserve(executables.size());
    for (const auto& [display, _] : executables)
        menu.emplace_back(display);

    for (;;) {
        int sel = -1;
        {
            NcursesUI ui;
            ui.drawFrame({}, ui.getSize());
            ui.refresh();

            std::string title = "Game Launcher [" 
                + std::to_string(ui.getWidth()) + "x" + std::to_string(ui.getHeight()) + "]";
            sel = ui.showOptionsDialog(title, menu);
        }
            
        const auto& [_, prog] = executables[sel];

        int res = run_program(prog);
        if (res < 0)
            std::perror("run_program");
    }
}
