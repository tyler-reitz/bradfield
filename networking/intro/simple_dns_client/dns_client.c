#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct headers {
  short id;
  short flags;
  short qd_count;
  short an_count;
  short ns_count;
  short ar_count;
};

struct question {
  char *name;
  short type;
  short class;
};

struct query {
  struct headers headers;
  struct question question;
};

int main(char argc, char *argv[]) {
  if (argc < 2) {
    perror("a.out wikipedia.org");
    exit(1);
  }

  struct headers headers;
  memset(&headers, htons(0), sizeof(headers));
  headers.id = htons(42);
  headers.flags = htons(0x0100);

  struct question question;

  int len = strlen(argv[1])+2;
  char name[len];
  char *cur = name;

  for (char *tok=strtok(argv[1], "."); tok!=NULL; tok=strtok(NULL, ".")) {
    *cur++ = strlen(tok);
    cur = stpcpy(cur, tok);
  } 
  question.name = name;
  question.type = htons(1);
  question.class = htons(1);

  struct query query;
  query.headers = headers;
  query.question = question;

  char buf[sizeof(struct query)];
  memcpy(buf, &query, sizeof(query)); 

  struct addrinfo hints, *res, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    perror("socket");
    exit(1);
  }
}
