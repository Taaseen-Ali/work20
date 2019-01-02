#include "pipe_networking.h"
#include <signal.h>

static void on_close(int signo){
  printf("[SERVER] Closing server...");
  remove("mario");
  exit(0);
}

int main() {
  int to_client;
  int from_client;
  signal(SIGINT, on_close);
  printf("==============WARNING==============\n");
  printf("THIS SERVER TURNS ALL C'S INTO B'S\n");
  printf("===================================\n\n");
  while(1){

    mkfifo("mario", 0644);
    printf("[SERVER] Waiting for client...\n");
    from_client = open("mario", O_RDONLY);
    printf("[SERVER] Client has connected\n");

    int f = fork();

    if(f){
      printf("[SERVER] remaking well known pipe\n");
      remove("mario");
      close(from_client);
    }

    else{
      subserver_handshake(&to_client, from_client);

      char received[BUFFER_SIZE];
      char to_send[BUFFER_SIZE];
      while(read(from_client, received, BUFFER_SIZE)>0){
	printf("[SERVER] Message received: %s", received);
	strcpy(to_send, received);
	int i=0;
	while(to_send[i]){
	  if(!strncmp(&to_send[i], "c", 1))
	    to_send[i] = 'b';
	  i++;
	}
	
	write(to_client, to_send, BUFFER_SIZE);
      }
      close(to_client);
      printf("[SERVER] Client disconnected\n");
    }
  }
}
