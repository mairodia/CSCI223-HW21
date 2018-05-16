// ==================================================================
// File: client01.c (Spring 2018)
// ==================================================================
// Programmer: Jennifer King
// Date: 5/15/2018
// Project: Programming with Unix Sockets
// Professor: Scott Edwards
// Class: CSCI 223 F | TR 4:30PM
// Description::
//      The server does the following:
//      -- delete the local socket file, if it exists
//              -- create a socket
//              -- bind the socket to the local socket file
//              -- put the socket in a listening state
//              -- wait for a client connection
//
//      Once a client connects, the server recieves a string message
//      from the client and writes it to stdout. Then the server
//      closes the conncetion and shuts down.
// ==================================================================

#include        <sys/types.h>
#include        <sys/socket.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/un.h>
#include        <unistd.h>
#include        <string.h>

#define LEN                             256
#define SOCKET_NAME             "server_socket"

// == main ==========================================================
//
// ==================================================================
int     main(void)
{
    auto        int                     socketID;
    auto        struct sockaddr_un      client;
    auto        char                    buf[LEN];
    auto        char*                   result;

    //Create an AF_UNIX/SOCK_STREAM socket
    socketID = socket(AF_UNIX, SOCK_STREAM, 0);

    //create structure and name members
    client.sun_family = AF_UNIX;
    strcpy(client.sun_path, SOCKET_NAME);

    if(connect(socketID, (struct sockaddr*)&client, sizeof(client)))
    {
        perror("client -- connect failed");
        exit(EXIT_FAILURE);
    }

    //get string of text from user
    while(1)
    {
        result = fgets(buf, LEN, stdin);
        if((NULL == result) || ('\n' == *result))
        {
            continue;
        }
        else
        {
            //send user text
            send(socketID, buf, LEN, 0);

            //if "quit"
            if(0 == strcmp(buf, "quit\n"))
            {
                close(socketID);
                break;
            }
        }
    }

    return 0;
}
