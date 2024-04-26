#include <stdio.h>
#include <stdlib.h>

char *cmd =
    "curl -X POST https://api.opentreeoflife.org/v3/tree_of_life/subtree -H "
    "\"content-type:application/json\" -d '{\"node_id\":\"ott93302\", "
    "\"height_limit\": 1, \"format\": \"arguson\"}'";

int main( int argc, char *argv[] )
{

  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    printf("%s", path);
  }

  /* close */
  pclose(fp);

  return 0;
}
