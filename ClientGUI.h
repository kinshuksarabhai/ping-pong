#ifndef CLIENT_GUI
#define CLIENT_GUI

#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include"GameStructures.h"
#include"GameState.h"
#include"NetworkClient.h"

class ClientGUI
{
    public:
        void display();
        void display_board();
        void display_paddles();
        void display_ball(int);
        void display_msg();
        void display_text(char*,float,float);
};

extern GameState gstate;
extern NetworkClient client;
void* cgui_main(void*);

#endif
