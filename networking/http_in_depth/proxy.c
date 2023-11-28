#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
  struct sockaddr_in local_addr;

  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(8080);
  local_addr.sin_addr.s_addr = INADDR_ANY;

  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  int yes=1;
  setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes); 

  bind(sfd, (struct sockaddr *)&local_addr, sizeof(local_addr));
  listen(sfd, 8);

  char *keep_alive;
  while(1) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len;
    int cfd = accept(sfd, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    printf("new connection!\n");

    char in[4096];
    ssize_t in_len = 0;
    char *headers, *body;
    do {
      keep_alive = NULL;
      in_len += recv(cfd, in+in_len, 4096-in_len, 0);
      printf("-> *    %x\n", in_len);

      if (!in_len) {
        keep_alive = NULL;
        break;
      }

      if (!headers) headers = strpbrk(in, "\r\n");
      if (!headers) continue;
      if (!keep_alive) keep_alive = strstr(in, "HTTP/1.1");

      if (!body) {
        if (*headers == '\r')
          body = strstr(headers, "\r\n\r\n");
        else
          body = strstr(headers, "\n\n");
      }
      if (!body) continue;

      if (!keep_alive) keep_alive = strstr(headers, "keep-alive");
      else if (strstr(headers, "Connection: close")) keep_alive = 0;

      struct sockaddr_in upstr_addr;
      upstr_addr.sin_family = AF_INET;
      upstr_addr.sin_port = htons(9000);
      upstr_addr.sin_addr.s_addr = htonl(0x7F000001);

      int upstr_fd = socket(AF_INET, SOCK_STREAM, 0);
      if (connect(upstr_fd, (struct sockaddr *)&upstr_addr, sizeof(upstr_addr)) == -1) {
        perror("Upstream");
        send(cfd, "HTTP/1.1 502 Bad Gateway\r\n\r\n", 28, 0);
        close(cfd);
        break;
      };

      ssize_t sent = send(upstr_fd, in, in_len, 0);
      printf("   * -> %x\n", in_len);

      char out[4096];
      ssize_t out_len;
      do {
        out_len = recv(upstr_fd, out, 4096, 0);
        printf("   * <- %x\n", out_len);

        if (out_len == 0) break;

        send(cfd, out, out_len, 0);
        printf("<- *    %x\n", out_len);
      } while(out_len);

      in_len = 0;
      headers = body = NULL;
      memset(in, 0, sizeof(in));
    } while (in_len || keep_alive);

    close(cfd);
  }
}

