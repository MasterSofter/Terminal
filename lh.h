#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

//цикл командной оболочки
void lsh_loop(void);
//чтение строки
char* lsh_read_line(void);
//парсинг строки
char** lsh_split_line(char *line);
//запуск программы
int lsh_launch(char** args);
//запускает либо встроенную комманду оболочки либо программу
int lsh_execute(char** args);

/*************************||******************************\
 *       Область встроенных комманд внутри оболочки
 * *******************************************************/

//Встроенная команда cd
int lsh_cd(char** args);
//Встроенная команда help
int lsh_help(char** args);
//Встроенная команда exit
int lsh_exit(char** args);

/*
  Список встроенных команд, за которыми следуют соответствующие функции
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}