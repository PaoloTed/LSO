#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(void){
int fd = open("text.txt", O_RDWR);
struct stat filestat;
fstat(fd, &filestat);
printf("%d", filestat.st_mode);
char buff[] = "Ugo 999\n";
lseek(fd, 0, SEEK_END);
int nbytes = write(fd, buff, strlen(buff));
if(nbytes == -1){
  perror("write");
}

dup2(STDOUT_FILENO, fd);
write(fd, buff, strlen(buff));
if(nbytes == -1){
  perror("write");
}
printf("\n%d\n",fd);
close(fd);
return 0;
}
