#include "pipe_networking.h"
#include <stdio.h>

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  char string[BUFFER_SIZE];
  char response[BUFFER_SIZE];
  while(1){
    printf("[CLIENT]: ");
    fgets(string, BUFFER_SIZE, stdin);
    write(to_server, string, BUFFER_SIZE);
    read(from_server, response, BUFFER_SIZE);
    printf("[SERVER] %s", response);
  }
}
