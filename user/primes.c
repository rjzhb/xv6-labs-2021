#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R 0
#define W 1

int
main(int argc, char *argv[])
{
  int i, p[2], prime = 2, BUF, iter = 0;
  pipe(p);

  for (i = 2; i <= 35; i++) {
    write(p[W], &i, sizeof(i));
  }

  while (read(p[R], &prime, sizeof(prime)) && prime > 1) {
    if (fork() == 0) {
      // add prime to the end of the queue for detecting END
      write(p[W], &prime, sizeof(BUF)); 
      for (; read(p[R], &BUF, sizeof(BUF)); iter ++) {
        if (BUF == prime)  {  // detecting END, avoid dead loop
          if (iter == 1)      // only 1 left; stop the big while loop 
            write(p[W], &iter, sizeof(BUF)); 
          break;
        }
        if (BUF % prime != 0) 
          write(p[W], &BUF, sizeof(BUF));
      }
      close(p[R]);
      close(p[W]);
      
      exit(0);
    } else {
      printf("prime %d\n", prime);
      wait(0);
    }
  }

  close(p[W]);
  close(p[R]);
  exit(0);
}
