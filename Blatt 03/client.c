#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "message.h"

/*
 * Tim Altenburg, 420632
 * Stephan Thiemicke, 375485
 * Steen Sziegaud, 384705
*/
void err(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

// create a new socket
int createSocket()
{
    // create socket
    int soc = socket(PF_INET, SOCK_STREAM, 0);
    if (soc < 0) {
        err("socket not created\n");
    }

    return soc;
}

// connect given socket soc to server at "localhost" on port "2342"
void connectToTimeServer(int soc)
{
    struct addrinfo hints;
    struct addrinfo *info;

    // setting attributes to specify connection to server
    memset(&hints, 0, sizeof(hints));
    // IP protocol
    hints.ai_family = AF_INET;
    // stream socket (byte stream)
    hints.ai_socktype = SOCK_STREAM;
    // active socket which can proactively connect to listening sockets
    hints.ai_flags = 0;

    // get address information of localhost:2324 with specified attributes 
    int retval = getaddrinfo("localhost", "2342", &hints, &info);
    if (retval != 0) {
        printf("getaddrinfo failed %s\n", gai_strerror(retval));
        exit(EXIT_FAILURE);
    }

    // connects socket to a server socket with address info
    retval = connect(soc, info->ai_addr, info->ai_addrlen);
    freeaddrinfo(info);
    if (retval < 0) {
        close(soc);
        err("connect failed");
    }
}

// send time request message to the server
void sendTimeRequest(int soc, const time_request_t* timeRequest)
{
    // send timeRequest through connection via socket
    int bytessend = send(soc, (const void*)timeRequest, sizeof(time_request_t), 0);
    if (bytessend < 0) { 
        close(soc); 
        err("error while sending"); 
    }
}

// receive time respond message from the server and store it in timeRespond
void receiveTimeRespond(int soc, time_respond_t* timeRespond)
{
    // recieve timeResponse through connection via socket
    int bytesrcvd = recv(soc, (void *)timeRespond, sizeof(time_respond_t), 0);
    if (bytesrcvd < 0) { 
        close(soc); 
        err("error while receiving"); 
    }
}

int main(int argc, char *argv[])
{
    // "handle" warnings when compiling
    char *prog = argv[0];
    (void)argc;
    prog = prog;

    // create client socket
    int soc = createSocket();
    connectToTimeServer(soc);

    time_request_t timeRequest;
    time_respond_t timeRespond;

    timeRequest.timezone = +1; // CET
    sendTimeRequest(soc, &timeRequest);

    receiveTimeRespond(soc, &timeRespond);
    printf("Current time in Münster: %s", timeRespond.time);


    timeRequest.timezone = -5; // EST
    sendTimeRequest(soc, &timeRequest);

    receiveTimeRespond(soc, &timeRespond);
    printf("Current time in New York: %s", timeRespond.time);


    // send disconnect message
    timeRequest.timezone = 127; // magic number
    sendTimeRequest(soc, &timeRequest);

    close(soc);

    return 0;
}

