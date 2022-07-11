#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[]) 
{
  int i;
  char *_argv[MAXARG], buf[1024], *argp, tmp, *arg_start;
  arg_start = argp = &buf[0];

  for (i = 0; i < argc-1; i++) _argv[i] = argv[i+1];

  while (read(0, &tmp, 1)) {
    if (tmp == ' ') {
      *argp++ = 0;
      _argv[i++] = arg_start;
      arg_start = argp;
      continue;
    } else if (tmp == '\n') {
      *argp++ = 0;
      _argv[i++] = arg_start;
      if (fork() == 0) {
        _argv[i] = 0x0;
        exec(_argv[0], _argv);
      } else {
        arg_start = argp;
        i = argc - 1;
        wait(0);
      }
    } else {
      *argp++ = tmp;
    }
  }
  exit(0);
}
