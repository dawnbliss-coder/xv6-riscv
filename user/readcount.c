#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd;
  char buf[100];
  int before, after;

  before = getreadcount();
  printf("Initial read count: %d\n", before);

  fd = open("README", O_RDONLY); // use an existing file in xv6 fs
  if(fd < 0){
    printf("Failed to open file\n");
    exit(1);
  }

  read(fd, buf, sizeof(buf));
  close(fd);

  after = getreadcount();
  printf("Read count after reading 100 bytes: %d\n", after);

  printf("Bytes increased: %d\n", after - before);

  exit(0);
}
