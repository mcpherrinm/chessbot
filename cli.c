#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "cli.h"

#define CMD(X) int X(int, char**);
#include "commands.inc"
#undef CMD

int cmd_dispatch(int argc, char **argv) {
  struct table_entry {
    char name[24];
    cmd_impl the_cmd;
  };

  static struct table_entry cmdtable[] = {
#define CMD(X) { #X , X },
#include "commands.inc"
#undef CMD
    {"", 0}
  };

  for(int i = 0; i < sizeof(cmdtable)/sizeof(*cmdtable); i++) {
    if(0 == strcmp(cmdtable[i].name, argv[0])) {
      return cmdtable[i].the_cmd(argc, argv);
    }
  }
  printf("%s: unknown command\n", argv[0]);
  return 0;
}

int cli(void) {
  while(1) {
    char *line = readline("ARM> ");
    if(!line) return 0;
    if (!*line) continue;

    const int ARG_MAX = 32;
    char *argv[ARG_MAX];
    argv[0] = strtok(line, " ");

    if(!argv[0]) continue;
    add_history(line);

    if(0 == strcmp(argv[0], "exit")) {
      return 0;
    }

    unsigned argc = 0;
    /* The -1 is for the null at the end */
    while((++argc < ARG_MAX-1) && (argv[argc] = strtok(NULL, " ")));
    argv[argc] = 0;

    if(cmd_dispatch(argc, argv)) {
      return 1;
    }

    free(line);
  }
  return -66; /*wtf*/
}
