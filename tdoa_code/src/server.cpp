/**
* Networking Lab
* CS 241 - Fall 2018
*/

#include "tdoa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
//#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <signal.h>

#define BUFFER_SIZE 1025

unsigned char buffer[10];

static int epfd, sock_fd, csock;

static bool sig_sent = false;

void sig_pipe(){
    sig_sent = true;
    shutdown(csock, SHUT_RDWR);
    close(csock);
}

bool sendData(char *buf){

  int size = buf[0] + 1;
  int tries = 10;
  while(1){
    tries--;
    int tmp = write(csock, buf, size);
    size -= tmp;
    if(tmp == -1 || tries == 0){
    sig_pipe();
    return false;
    }
    if(size <= 0) {return true;}
  }

}

void handler(){

    shutdown(csock, SHUT_RDWR);
    close(csock);
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);

    exit(0);
}

int main(int argc, char const *argv[])
{

  Pair p1;

  if(argc < 2)
  {
    printf("Usage: <executable> <PORT NUMBER>\n");
    return -1;
  }

  signal(SIGINT, handler);
  signal(SIGPIPE, sig_pipe);
  char *str = "126.44,32.5#";
  int s;
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  s = getaddrinfo(NULL, argv[1], &hints, &result);

  if (s != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
      exit(1);
  }

  int optval = 1;
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof (optval));

  if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
      perror("bind()");
      exit(1);
  }

  if (listen(sock_fd, 10) != 0) {
      perror("listen()");
      exit(1);
  }

  struct sockaddr client;
  socklen_t client_len;



  memset(buffer, 0, sizeof(buffer));

  int read_len = 0;

  while(1){
    x:
        sig_sent = false;
        csock = accept(sock_fd, &client, &client_len);
        if(csock == -1)
            goto x;
        while(1){

          read_len = read(csock, &buffer, 10);

          if(sig_sent || (read_len <= 0) || (buffer[0] != 1))
              goto x;

          buffer[0] = strlen(str);

          memcpy(&buffer[1], str, strlen(str));

          if(!sendData(buffer))
          {
            sig_pipe();
            goto x;
          }

          printf("%s %d \n", str, buffer[0]);

        }

  }

}
