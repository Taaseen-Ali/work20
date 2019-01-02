#include "pipe_networking.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int subserver_handshake(int *to_client, int from_client) {
  int in_pipe = from_client;
  char string[HANDSHAKE_BUFFER_SIZE];
  
  read(in_pipe, string, HANDSHAKE_BUFFER_SIZE);
  printf("[SERVER] Message received: %s\n", string);
    
  int out_pipe = open(string, O_WRONLY);
  write(out_pipe, ACK, HANDSHAKE_BUFFER_SIZE);
  printf("[SERVER] Sent string '%s'\n", ACK);
  
  read(in_pipe, string, HANDSHAKE_BUFFER_SIZE);
  printf("[SERVER] Message received: %s\n", string);
 
  * to_client = out_pipe;
  return in_pipe;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("luigi", 0644);
  int out_pipe = open("mario", O_WRONLY);
  if(out_pipe == -1){
    printf("[CLIENT] Server pipe does not exist\n");
    remove("luigi");
    return 1;
  }

  printf("[CLIENT] Connected to server\n");
  
  
  write(out_pipe, "luigi", HANDSHAKE_BUFFER_SIZE);
  printf("[CLIENT] Sent string 'luigi'\n");

  int in_pipe = open("luigi", O_RDONLY);
  char string[HANDSHAKE_BUFFER_SIZE];
  read(in_pipe, string, HANDSHAKE_BUFFER_SIZE);
  printf("[CLIENT] Message received: %s\n", string);
  remove("luigi");
  
  write(out_pipe, "done", HANDSHAKE_BUFFER_SIZE);
  * to_server = out_pipe;
  printf("[CLIENT] Sent string 'done'\n");
  return in_pipe;
}
