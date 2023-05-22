#include <stdio.h>
#include "index.h"

int main(int argc, char *argv[]){
  if (argc != 3){
    fprintf(stderr, "Come on -- 3 args");
    return 1;
  }

  index_loader(argv[1], argv[2]);

  return 0;


}
