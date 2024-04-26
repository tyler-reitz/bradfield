/*
 * Explore the tree of life in your filesystem, using macFUSE
 *
 * [Download](https://osxfuse.github.io/) and install macFUSE using the dmg
 * installer (note, you will need to give permission via the Security & Privacy
 * pane of System Preferences, when prompted) then restart your computer
 *
 * Then compile:
 *
 *     clang -Wall lifefs.c `pkg-config fuse --cflags --libs` -o lifefs
 *
 * To run:
 *     mkdir life
 *     ./lifefs life/
 *
 * You should then be able to ls life and go from there!
 *
 * To unmount, just `umount life/`
 */

#define FUSE_USE_VERSION 26

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

int sockfd; // fd for our socket to the Python coprocess
struct sockaddr_un dest_addr; // destination of the Python coprocess
socklen_t addrlen;
FILE *logp; // fd of log file
time_t now; // used for logging
char stime[20]; // used for logging
char children[1024][1024]; // buffer of results of readdir etc

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define LOG(fmt, str)                                                          \
  time(&now);                                                                  \
  fprintf(logp, ANSI_COLOR_GREEN);                                             \
  strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M:%S", localtime(&now));        \
  fprintf(logp, "%s: ", stime);                                                \
  fprintf(logp, fmt, str);                                                     \
  fprintf(logp, ANSI_COLOR_RESET);                                             \
  fflush(logp);

// Retrieve the child taxa for the given taxon path
// Calls out to a little Python script to make an API call
int get_children(const char *path) {
  LOG("Sending %s\n", path);
  if (sendto(sockfd, path, strlen(path), 0, (const struct sockaddr *)&dest_addr,
             addrlen) == -1) {
    LOG("send error [%d]\n", errno);
    LOG("%s\n", strerror(errno));
  }

  int i = 0;
  for (;;) {
    int n = recv(sockfd, children[i], 1024, 0);
    children[i][n] = '\0';
    if (children[i][0] == '.') {
      break;
    }
    LOG("Received %s\n", children[i]);
    i += 1;
  }

  return i;
}

static int life_getattr(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));
  // TODO actually check validity and -ENOENT if no entry
  // TODO handle leaf nodes as files (st_mode = S_IFREG)
  stbuf->st_mode = S_IFDIR | 0755;
  stbuf->st_nlink = 2;
  return 0;
}

static int life_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi) {
  (void)offset;
  (void)fi;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  int n = get_children(path);
  for (int i = 0; i < n; i++) {
    filler(buf, children[i], NULL, 0);
    LOG("readdir filled %s\n", children[i]);
  }
  return 0;
}

static int life_open(const char *path, struct fuse_file_info *fi) {
  if ((fi->flags & O_ACCMODE) != O_RDONLY)
    return -EACCES;

  return 0;
}

static int life_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi) {
  (void)fi;
  return 0;
}

static const struct fuse_operations life_oper = {
    .getattr = life_getattr,
    .readdir = life_readdir,
    .open = life_open,
    .read = life_read,
};

int main(int argc, char *argv[]) {
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  logp = fopen("log", "a");

  LOG("%s\n", "Starting");
  // Before starting fuse, set up a UNIX domain socket to
  // communicate with our Python coprocess, and run the latter
  struct sockaddr_un source_addr;
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  source_addr.sun_family = AF_UNIX;
  dest_addr.sun_family = AF_UNIX;
  strcpy(source_addr.sun_path, "/tmp/lifesock-client");
  strcpy(dest_addr.sun_path, "/tmp/lifesock-server");
  remove(source_addr.sun_path);
  remove(dest_addr.sun_path);
  bind(sockfd, (struct sockaddr *)&source_addr,
       strlen(source_addr.sun_path) + sizeof(source_addr.sun_family) + 1);
  addrlen = strlen(dest_addr.sun_path) + sizeof(dest_addr.sun_family) + 1;

  if (fork() == 0) {
    execlp("python3", "python3", "buddy.py", (char *)0);
    LOG("execl error [%d]\n", errno);
    LOG("%s\n", strerror(errno));
    return -1;
  }

  return fuse_main(args.argc, args.argv, &life_oper, NULL);
}
