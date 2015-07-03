/*******************************************************************************
* NullidentdMod is a bare minimum identd server for use alongside systemd
* Copyright (C) 2014-2015 - AcidHub <acidhub@craft.net.br>
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define SESSION_TIMEOUT 10

void write_response(int fd, char *response, int len) {
    int retval;
    int byteswritten = 0;

    while(byteswritten < len) {
        retval = write(fd, response + byteswritten, len - byteswritten);

        if(retval <= 0) {
            break;
        }

        byteswritten += retval;
    }
}

void read_request(int fd, char *request, int maxlen) {
    char c;
    int bytesread = 0;

    /* read until end of bytes or \n */
    while(bytesread < maxlen - 1) {
        if(read(fd, &c, 1) != 1) {
            exit(EXIT_SUCCESS);
        }

        if(c == '\n') {
            /* end of line */
            break;
        }

        request[bytesread] = c;
        bytesread += 1;
    }

    if(bytesread > 0 && request[bytesread-1] == '\r') {
        /* strip trailing \r */
        bytesread -= 1;
    }

    /* null terminate */
    request[bytesread] = '\0';
}

void read_random(char *buffer, size_t size) {
    char *p = buffer;

    if (!buffer || size < 2) {
        exit(EXIT_FAILURE);
    }

    FILE *randfd = fopen("/dev/urandom", "r");

    /* Fallback to /dev/random
       if /dev/urandom not found */
    if (!randfd) {
        randfd = fopen("/dev/random", "r");
        if (!randfd) {
            fprintf(stderr, "Not enough random data, it's possible?.\n");
            exit(EXIT_FAILURE);   
        }
    }

    while (strlen(buffer) < size) {
        int c = fgetc(randfd);

        if (ferror(randfd) != 0 || feof(randfd) != 0) {
            break;
        }

        if (c < 1) { 
            continue;
        }

        sprintf(p, "%02x", c);
        p += 2;
    }
}

void signal_handler(int signum) {
    fprintf(stderr, "Session timeout.\n");
    exit(EXIT_FAILURE);
}

// C99 standard
int main(int argc, char *argv[]) {
    int infd;
    int outfd;
    int response_len;
    char response[256];
    char request[129];
    char data[9];

    if(getuid() == 0) {
        fprintf(stderr, "User id is root, exiting.\n");
        return EXIT_FAILURE;
    }

    infd = fileno(stdin);
    outfd = fileno(stdout);

    // set the session timeout
    signal(SIGALRM, signal_handler);
    alarm(SESSION_TIMEOUT);

    for(;;) {
        memset(response, 0, sizeof(response));
        memset(request, 0, sizeof(request));
        memset(data, 0, sizeof(data));
        
        /* read the request */
        read_request(infd, request, sizeof(request));

        if(argv[1] == 0) {
            read_random(data, 8);
        } else {
            sprintf(data, "%s", argv[1]);
        }

        /* send the line */
        response_len = snprintf(response, sizeof(response), "%.20s : USERID : UNIX : %.20s\r\n", request, data);
        write_response(outfd, response, response_len);
        break; // send only one time and close
    }
    return EXIT_SUCCESS;
}

