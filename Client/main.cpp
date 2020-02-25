// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1153
#define BUFLEN 2048
#define MSGS 1000	/* number of messages to send */

int main(void)
{
  struct sockaddr_in myaddr, remaddr;
  int fd, i, slen=sizeof(remaddr);
  char *server = "127.0.0.1";	/* change this to use a different server */
  char buf[BUFLEN];

  // Create a UDP socket
  // SOCK_STREAM for TCP / SOCK_DGRAM for UDP
  if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    printf("socket created\n");

  /* bind it to all local addresses and pick any port number */

  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(0);

  if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
    return 0;
  }

  /* now define remaddr, the address to whom we want to send messages */
  /* For convenience, the host address is expressed as a numeric IP address */
  /* that we will convert to a binary format via inet_aton */

  memset((char *) &remaddr, 0, sizeof(remaddr));
  remaddr.sin_family = AF_INET;
  remaddr.sin_port = htons(PORT);
  if (inet_aton(server, &remaddr.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  /* now let's send the messages */

  for (i=0; i < MSGS; i++) {
    printf("Sending packet %d to %s port %d\n", i, server, PORT);
    sprintf(buf, "This is packet %d", i);
    if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
      perror("sendto");
  }
  close(fd);
  return 0;
}
