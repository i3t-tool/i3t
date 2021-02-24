#include "picoc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

int PicocRunInteractive();
int PicocRunFile(char* filename);
int PicocRunSource(char* source);

int picoc(int argc, char** argv){
  int ret;
  if (argc == 1){
    ret = PicocRunInteractive();
  }
  else if (argc == 2){
    if (strcmp("-s", argv[1]) == 0){ret = PicocRunSource("printf(\"deutschland siegt!\");");}
    else{ret = PicocRunFile(argv[1]);}
  }
  else if (argc == 3){
    // picocrunfiles
  }
  printf("ret %d\n", ret);
  return ret;
}
int PicocRunInteractive(){
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  PicocIncludeAllSystemHeaders(&pc);
  PicocParseInteractive(&pc);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
}
int PicocRunFile(char* filename){
  FILE* f = fopen(filename, "r");
  if (f == NULL){return -1;}
  fclose(f);
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  PicocIncludeAllSystemHeaders(&pc);
  PicocPlatformScanFile(&pc, filename);
  PicocCleanup(&pc);
  return pc.PicocExitValue;

  // for (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++){PicocPlatformScanFile(&pc,
  // argv[ParamCount]);}//run multiple files PicocCallMain(&pc, argc - ParamCount, &argv[ParamCount]);	//call main
}
int PicocRunSource(char* source){
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  PicocIncludeAllSystemHeaders(&pc);
  PicocParse(&pc, "PicocRunString", source, strlen(source), TRUE, TRUE, TRUE, TRUE);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
}