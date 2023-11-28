#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#define BUF_SIZE 500

int main(int argc, char *argv[]) {
  int sfd, s;
  char buf[BUF_SIZE];
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  s = getaddrinfo(NULL, argv[1], &hints, &res); 

  struct addrinfo *rp;
  for (rp=res; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (sfd == -1)
      continue;

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;

    close(sfd);
  }

  freeaddrinfo(res);

  socklen_t peer_addrlen;
  struct sockaddr_storage peer_addr;
  for (;;) {
    char host[NI_MAXHOST], service[NI_MAXSERV];

    peer_addrlen = sizeof(peer_addr);
    int n = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &peer_addrlen);
    getnameinfo((struct sockaddr *) &peer_addr, peer_addrlen, host, NI_MAXHOST,
                service, NI_MAXSERV, NI_NUMERICSERV);

    printf("Received %zd bytes from %s:s\n", n, host, service);
    for (int i=0; i<n; i++) buf[i] = toupper(buf[i]);       
    sendto(sfd, buf, n, 0, (struct sockaddr *) &peer_addr, peer_addrlen);
  }
}
