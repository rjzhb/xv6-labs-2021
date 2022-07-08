#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
  int p[2];
  char BUF[1];
  pipe(p);

  if (fork() == 0) {
    if (read(p[0], BUF, 1))
      printf("%d: received ping\n", getpid());
    close(p[0]);
    write(p[1], "1", 1);
    exit(0);
  } else {
    write(p[1], "1", 1);
    close(p[1]);
    if (read(p[0], BUF, 1))
      printf("%d: received pong\n", getpid());
    exit(0);
  }
}
