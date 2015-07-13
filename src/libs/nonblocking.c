/* C-noise: The open source noise generator  */ 
/* Copyright (C) 2015  Michael Winters  */
/*
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or  */
/* (at your option) any later version.  */
/*
/* This program is distributed in the hope that it will be useful,  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  */
/* GNU General Public License for more details.  */
/*
/* You should have received a copy of the GNU General Public License  */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <termios.h>
#include <unistd.h>
#include <sys/types.h> 

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
void nonblock(int state)
{
    struct termios ttystate;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
 
    if (state==1)
    {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
	ttystate.c_lflag &= ~ECHO;
    }
    else if (state==0)
    {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
	ttystate.c_lflag |= ECHO;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

