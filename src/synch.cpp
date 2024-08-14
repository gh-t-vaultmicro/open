#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void run_process(const char* program, const char* arg1 = nullptr, const char* arg2 = nullptr, const char* arg3 = nullptr, const char* arg4 = nullptr, 
                 const char* arg5 = nullptr, const char* arg6 = nullptr, const char* arg7 = nullptr, const char* arg8 = nullptr, const char* arg9 = nullptr,
                 const char* arg10 = nullptr, const char* arg11 = nullptr, const char* arg12 = nullptr, const char* arg13 = nullptr, const char* arg14 = nullptr,
                 const char* arg15 = nullptr, const char* arg16 = nullptr, const char* arg17 = nullptr, const char* arg18 = nullptr, const char* arg19 = nullptr) {

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        exit(1);
    }

    if (pid == 0) {
        // child process
        execl(program, program, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, (char*)NULL);
        cerr << "execl failed!" << endl;
        exit(1);
    }
}

int main() {
    run_process("../open/build/openmt", "-d", "0", "-f", "30", "-w", "640", "-h", "480", "-m", "0", "-s", "0", "l", "0", "-v");

    //sleep so that the camera can be opened before the next process starts
    sleep(8);
    run_process("../pcap/build/pcaplpcq", "-d", "usbmon1", "-b", "1048576");

    // wait till child processes are done
    int status;
    while (wait(&status) > 0);

    return 0;
}
