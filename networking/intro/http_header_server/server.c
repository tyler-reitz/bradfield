#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "a.out PORT\n");
    exit(1);
  }

  struct addrinfo hints, *res, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int rv = getaddrinfo(NULL, argv[1], &hints, &res);
  if (rv != 0) {
    fprintf(stderr, "%s\n", gai_strerror(rv));
    exit(1);
  };

  int sfd;
  for (p = res; res != NULL; res = res->ai_next) {
    sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);  

    if (sfd == -1) {
      perror(NULL);
      exit(1);
    }

    if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror(NULL);
      exit(1);
    }

    break;
  }

  freeaddrinfo(res);

  if (p == NULL) {
    fprintf(stderr, "Server failed to listen\n");
    exit(1);
  }

  if (listen(sfd, 10) == -1) {
    perror(NULL);
    exit(1);
  } 

  printf("server: waiting for connection...\n");

  for (;;) {
    struct sockaddr client_addr;
    socklen_t client_addr_len;

    int sfd2 = accept(sfd, &client_addr, &client_addr_len);

    if (sfd2 == -1) {
      perror(NULL);
      exit(1);
    }

    if (!fork()) {
      close(sfd);

      int n;
      char buf[100];

      while ((n = recv(sfd2, buf, 100, 0)) != 0) {
        printf("%s", buf);
        // for (int i = 0; i < n; i++) buf[i] = toupper(buf[i]);
        if (strstr(buf, "\r\n\r/n") != NULL) {
          send(sfd2, buf, n, 0); 
          break;
        }
        send(sfd2, buf, n, 0); 
      }
    }
    close(sfd2);
  }
}
