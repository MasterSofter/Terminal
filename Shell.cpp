#include "Shell.h"


Shell::Shell()
{
  
}


int sh_ctr_c(pid_t pid)
{
    return 1;
}

int Shell::sh_help(char **args)
{
  int i;
  printf("Pavel Korolev's SH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < 3; i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int Shell::sh_exit(char** args)
{
    return 0;
}

pid_t childPid;
void kill(int sig)
{
    //здесь будет мой код
    if(childPid != 0)
      kill(childPid, sig);

    printf("\n");
}

void Shell::sh_loop()
{
  char* line;
  char** args;
  int status;

  struct sigaction act;
    act.sa_handler = kill;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

  do {
    printf("> ");
    line = sh_read_line();      //Читаем строку
    printf("%s\n",line);
    args = sh_parse_line(&line[0]); //Парсим строку
    status = sh_execute(args);  //Выполняем программу

    free(line);                 //Освобождаем память
    free(args);
  } while (status);             

}

int Shell::sh_cd(char **args)
{
  if (args[1] == NULL) {
    printf("sh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("sh");
    }
  }
  return 1;
}

int Shell::sh_execute(char** args)
{

    if(strcmp(args[0], "help") == 0)
    {
        return sh_help(args);
    }

    if(strcmp(args[0], "cd") == 0)
    {
        return sh_cd(args);
    }
  

    if(strcmp(args[0], "exit") == 0)
    {
        return sh_exit(args);
    }


    int status;

    pid = fork();
    childPid = pid;
    if (pid == 0) 
    {
        // Child process
        if (execvp(args[0], args) == -1) 
            perror("sh");
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) 
    {
        // Error forking
        perror("sh");
    } 
    else {
        // Parent process
        do 
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

char* Shell::sh_read_line()
{
  int bufsize = SIZEBUFFER;
  int position = 0;
  char *buffer = (char*)malloc(sizeof(char) * bufsize);
  int symb;

  if (!buffer) {
    printf("sh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {

    symb = getchar();

    if (symb == '\0' || symb == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = symb;
    }
    position++;

    //Выделяем еще кусок памяти, если нам не хватило
    if (position >= bufsize) {
      bufsize += SIZEBUFFER;
      buffer = (char*)realloc(buffer, bufsize);
      if (!buffer) {
        printf("sh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char** Shell::sh_parse_line(char* line)
{
    
    int bufsize = SIZEBUFFER, position = 0;
    char** tokens = (char**)malloc(bufsize * sizeof(char*));
    char* token;

    if (!tokens) {
        printf("sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) 
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                printf("sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }
  
  tokens[position] = NULL;
  return tokens;

}
