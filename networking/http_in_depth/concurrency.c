#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <zlib.h>

#define MAX_CONN 10
#define BUFF_LEN 4096 * 4

int create_socket(struct sockaddr_in *addr, int port, int host) {
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  addr->sin_addr.s_addr = htonl(host);

  int lstnr = socket(AF_INET, SOCK_STREAM, 0);
  int yes=1;
  setsockopt(lstnr, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  return lstnr;
}

int main(int argc, char *argv) {
  struct sockaddr_in addr;
  int lstnr = create_socket(&addr, 8888, 0x7F000001);

  bind(lstnr, (struct sockaddr *)&addr, sizeof(addr));
  listen(lstnr, MAX_CONN);

  fd_set rds, wds;
  FD_ZERO(&rds);
  FD_ZERO(&wds);
  FD_SET(lstnr, &rds);

  int nfds = lstnr;
  int rdbl[MAX_CONN] = {0};
  int wrtbl[MAX_CONN] = {0};

  rdbl[lstnr]=1;

  struct req {
    int curr;
    char *buf;
    char *end;
    int keep_alive;
  };

  struct req reqs[MAX_CONN];
  struct req resps[MAX_CONN];

  while(1) {
    for (int i=0; i<=nfds; i++) {
      if (rdbl[i] == 1) FD_SET(i, &rds);
    }

    int n = select(nfds+1, &rds, NULL, NULL, NULL);

    for (int i=3; i<=nfds; i++) {
      if (FD_ISSET(i, &rds)) {
        if (i == lstnr) {
          // Listen for connections
          int cfd = accept(lstnr, NULL, NULL);
          rdbl[cfd]=1;
          nfds=cfd;

          reqs[cfd].curr = 0;
          reqs[cfd].end = 0;
          reqs[cfd].buf = malloc(4096);
          reqs[cfd].keep_alive = 0;
          reqs[cfd].end = NULL;
        } else {
          ssize_t data;

          // Read from clients
          char *buf = reqs[i].buf+reqs[i].curr;
          int len = BUFF_LEN - reqs[i].curr;

          data = recv(i, buf, len, 0);
          printf(" -> *    %03dB\n", data);
          reqs[i].curr += data;
          reqs[i].end = strstr(reqs[i].buf, "\r\n\r\n");
          if (!reqs[i].end) reqs[i].end = strstr(reqs[i].buf, "\n\n");
          reqs[i].keep_alive = 
            strstr(reqs[i].buf, "HTTP/1.1") || strstr(reqs[i].buf, "keep-alive");

          if (data) {
            if (reqs[i].end) {
              // Connect to upstream
              struct sockaddr_in addr;
              int upfd = create_socket(&addr, 9000, 0x7F000001);
              if (connect(upfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                send(i, "HTTP/1.1 502 Bad Gateway\r\n\r\n", 28, 0);
              };

              // Write to upstream
              printf("    * -> %03dB\n\n", strlen(reqs[i].buf));
              send(upfd, reqs[i].buf, reqs[i].curr, 0);

              resps[i].curr = 0;
              resps[i].end = 0;
              resps[i].buf = malloc(BUFF_LEN);

              // Read from upstream
              ssize_t udata;
              do {
                udata = recv(upfd, resps[i].buf+resps[i].curr, BUFF_LEN-resps[i].curr, 0); 
                printf("    * <- %03dB\n", udata);
                resps[i].curr += udata;
              } while (udata);

              wrtbl[i]=1;
              FD_SET(i, &wds);
            }
          } else { 
            rdbl[i]=0;
            FD_CLR(i, &rds);
            close(i);
          }
        }
      }
    }

    for (int i=3; i<=nfds; i++) {
      if (FD_ISSET(i, &wds)) {
        // Write to downstream
        char *gzip = strstr(reqs[i].buf, "gzip");
        char *image = strstr(resps[i].buf, "Content-type: image");
        if (!image && gzip) {
          char *cl = strstr(resps[i].buf, "Content-Length: ");
          char *cle = strpbrk(cl+16, "\r\n")+2;
          char *body = strstr(resps[i].buf, "\r\n\r\n")+4;

          size_t ucsz = resps[i].buf+resps[i].curr-body+1;
          size_t cb = compressBound(ucsz);
          uLongf csz = cb;
          Bytef *gzip = (Bytef *)malloc(cb);

          z_stream stream;
          stream.zalloc = Z_NULL;
          stream.zfree = Z_NULL;
          stream.opaque = Z_NULL;
          stream.avail_in = (uInt)ucsz;
          stream.next_in = (Bytef *)body;
          stream.avail_out = cb;
          stream.next_out = gzip;

          deflateInit(&stream, Z_DEFAULT_COMPRESSION);
          deflate(&stream, Z_FINISH);
          csz = csz-stream.avail_out;

          deflateEnd(&stream);
          char fmt[] = "Content-Length: %d\r\n";
          char gzl[30];
          int s = snprintf(gzl, 30, fmt, csz);

          send(i, resps[i].buf, cl-resps[i].buf, 0);
          send(i, gzl, s, 0);
          send(i, "Content-Encoding: deflate\r\n", 27, 0);
          send(i, cle, body-cle, 0);
          send(i, gzip, csz, 0);
          printf(" <- *    %03dB\n\n", body-resps[i].buf+csz);
          free(gzip);
        } else {
          send(i, resps[i].buf, resps[i].curr, 0);
          printf(" <- *    %03dB\n\n", strlen(resps[i].buf));
        }

        wrtbl[i]=0;
        FD_CLR(i, &wds);

        if (reqs[i].keep_alive) {
          reqs[i].curr=0;
          reqs[i].end=0;
          memset(reqs[i].buf, 0, BUFF_LEN);
          continue;
        };

        rdbl[i]=0;
        FD_CLR(i, &rds);
        close(i);
      }
    }
  };
}

