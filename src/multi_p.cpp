#include <iostream>
#include <unistd.h>  // fork, getpid
#include <sys/wait.h> // wait
#include <chrono>
#include <ctime>

void code1() {
    for (int i = 0; i < 5; ++i) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Code1 (PID: " << getpid() << ") - Time: " << std::ctime(&now_time) << " - Step " << i << std::endl;
        sleep(1);
    }
}

void code2() {
    for (int i = 0; i < 5; ++i) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Code2 (PID: " << getpid() << ") - Time: " << std::ctime(&now_time) << " - Step " << i << std::endl;
        sleep(1);
    }
}

int main() {
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // child process 1
        code1();
        return 0;
    } else {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // child process 2
            code2();
            return 0;
        } else {
            // parent process: wait till child process finishes
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }

    return 0;
}
