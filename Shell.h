#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <signal.h>
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
const int SIZEBUFFER = 1024;


class Shell
{
public:
    Shell();
    //цикл командной оболочки
    void sh_loop();
private:
    //чтение строки
    char* sh_read_line();
    //парсинг строки
    char** sh_parse_line(char* line);
    //запускает либо встроенную комманду оболочки либо программу
    int sh_execute(char** args);

 /*************************||******************************\
 *      Область встроенных комманд внутри оболочки
 * *******************************************************/
private:
    pid_t pid, wpid;
    //Встроенная команда cd
    int sh_cd(char** args);
    //Встроенная команда help
    int sh_help(char** args);
    //Встроенная команда exit
    int sh_exit(char** args);

    int sh_ctr_c(pid_t pid);

    char* builtin_str[3] = {
    "cd",
    "help",
    "exit"};

};