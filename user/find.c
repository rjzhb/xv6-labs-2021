#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *filename)
{
  char buf[512], *p, basename[DIRSIZ+1];
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat%s\n", path);
    close(fd);
    return;
  }
  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    fprintf(2, "find: path too long\n");
  if (st.type == T_FILE)
    fprintf(2, "find: %s is not a directory\n", path);

  // go searching
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0)
      continue;
    strcpy(basename, de.name);
    memmove(p, de.name, DIRSIZ); // cp de.name into p
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      fprintf(2, "ls: cannot stat %s\n", buf);
      continue;
    }
    switch(st.type) {
      case T_FILE: 
        if (!strcmp(basename, filename)) { // 0 -> equal strings
          printf("%s\n", buf);
        }
        break;
      case T_DIR:
        if (strcmp(basename, ".") == 0 || strcmp(basename, "..") == 0) continue;
        find(buf, filename);
        break;
    }
  }
  return;
}

int
main(int argc, char *argv[]) 
{
  if (argc != 3) {
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
