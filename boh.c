#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
int main(void) {
  int p1[2], p2[2];
  char buf;
  int c1 = 0, c2 = 0;
  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
    // TODO: chiude pipe p1 in lettura
    close(p1[0]) while (1) write(p1[1], "A", 1);
  }
  if (fork() == 0) {
    // TODO: chiude pipe p2 in lettura
    close(p2[0]) while (1) write(p2[1], "B", 1);
  }
  close(p1[1]);
  close(p2[1]);
  fd_set set;
  int maxfd = p1[0] > p2[0] ? p1[0] : p2[0];
  while (c1 + c2 < 10) {
    FD_ZERO(&set);
    FD_SET(p1[0], &set);
    FD_SET(p2[0], &set);
    /* TODO: aggiungere pipe al set di fd */
    signal(SIGPIPE, SIG_IGN) select(maxfd + 1, &set, 0, 0, 0);
    if (FD_ISSET(p1[0], &set))
      c1++;
    if (FD_ISSET(p2[0], &set))
      c2++;
    /* TODO: controllare pipe pronte e incrementare
    i contatori dei caratteri ricevuti */
  }
  printf("pipe1 = %d, pipe2 = %d\n", c1, c2);
  close(p1[0]);
  close(p2[0]);
  return 0

         due processi,
         e 2 thread nel padre,
         non ci sta wait(null) nel padre non ci sta
         pthread_cond_signal(&c) in f2

         1 f2
         v2 = 2 v1 = 2 2 f1 non si egegue a meno di risvegli spuri,
         correggendo con signal 2 f1 v1 = 3

                                          print del padre mettendo
                                          wait(NULL) 3 2 print del figlio 1 5

                                          ps
                                          - eo pid,
         nlwp,
         comm | sort - nr - k2 |
             head -
                 3

                 ps
                 - eo pid,
         nlwp | awk - v max =
             "$(ps -eo pid,nlwp | sort -nr -k2 | head "
             "-1)" '$2 == v {print $c1}' cat
             "$(ls -l | awk '^-' | sort -nr -k5 | head -1 | awk '{print $9}')" |
             sed 's/\([^\)].*\)/x/g'
