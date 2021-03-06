#include"ClientGUI.h"

void ClientGUI::display()
{
    //char title[25];
    //  sprintf(title,"Ping-Pong:Playing on Wall %d",gstate.wall_no);
    //  glutSetWindowTitle(title);
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0.5,0.5,0);
    glRotated(gstate.wall_no*90, 0, 0, 1);
    glTranslatef(-0.5,-0.5,0);
    display_board();
    display_paddles();
    for(int i=0;i<gstate.num_balls;i++)
        display_ball(i);
    glPopMatrix();
    display_msg();
    glutSwapBuffers();
    glFlush ();
}
void ClientGUI::display_board()
{
    glColor3f (0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f (0.0, 0.0, 0.0);
    glVertex3f (1.0, 0.0, 0.0);
    glVertex3f (1.0, 1.0, 0.0);
    glVertex3f (0.0, 1.0, 0.0);
    glEnd();
    glColor3f (0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex3f (PADDLE_WIDTH,PADDLE_WIDTH, 0.0);
    glVertex3f (1-PADDLE_WIDTH,PADDLE_WIDTH, 0.0);
    glVertex3f (1-PADDLE_WIDTH,1-PADDLE_WIDTH, 0.0);
    glVertex3f (PADDLE_WIDTH,1-PADDLE_WIDTH, 0.0);
    glEnd();
}
void ClientGUI::display_paddles()
{
    float x0,x1,y0,y1;
    glColor3f (1.0, 1.0, 1.0);

    //paddle 0
    x0=PADDLE_WIDTH+gstate.paddle[0].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
    x1=PADDLE_WIDTH+L*PADDLE_LENGTH+gstate.paddle[0].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
    y0=0;
    y1=PADDLE_WIDTH;
    glBegin(GL_POLYGON);
    glVertex3f (x0,y0, 0.0);
    glVertex3f (x1,y0, 0.0);
    glVertex3f (x1,y1, 0.0);
    glVertex3f (x0,y1, 0.0);
    glEnd();

    //paddle 1
    x0=1-PADDLE_WIDTH;
    x1=1;
    y0=PADDLE_WIDTH+gstate.paddle[1].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
    y1=PADDLE_WIDTH+L*PADDLE_LENGTH+gstate.paddle[1].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
    glBegin(GL_POLYGON);
    glVertex3f (x0,y0, 0.0);
    glVertex3f (x1,y0, 0.0);
    glVertex3f (x1,y1, 0.0);
    glVertex3f (x0,y1, 0.0);
    glEnd();

    //paddle 2
    x0=1-PADDLE_WIDTH+gstate.paddle[2].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
    x1=1-PADDLE_WIDTH-L*PADDLE_LENGTH+gstate.paddle[2].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
    y0=1-PADDLE_WIDTH;
    y1=1;
    glBegin(GL_POLYGON);
    glVertex3f (x0,y0, 0.0);
    glVertex3f (x1,y0, 0.0);
    glVertex3f (x1,y1, 0.0);
    glVertex3f (x0,y1, 0.0);
    glEnd();

    //paddle 3
    x0=0;
    x1=PADDLE_WIDTH;
    y0=1-PADDLE_WIDTH+gstate.paddle[3].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
    y1=1-PADDLE_WIDTH-L*PADDLE_LENGTH+gstate.paddle[3].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
    glBegin(GL_POLYGON);
    glVertex3f (x0,y0, 0.0);
    glVertex3f (x1,y0, 0.0);
    glVertex3f (x1,y1, 0.0);
    glVertex3f (x0,y1, 0.0);
    glEnd();

}
void ClientGUI::display_ball(int i)
{
    //filled circle
    float x1,y1,x2,y2;
    float angle;
    double radius=BALL_SIZE;

    //x1 = 0.5,y1=0.6;
    x1=PADDLE_WIDTH+gstate.ball[i].position.x*L;
    y1=PADDLE_WIDTH+gstate.ball[i].position.y*L;
    glColor3f(1.0,1.0,0.0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);

    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }

    glEnd();
}
void ClientGUI::display_msg()
{
    switch(gstate.status)
    {
        case GAME_INIT:
            display_text((char*)"Connecting to server...",0.3,0.6);
            break;
        case GAME_WAITING:
            display_text((char*)"Press <space> when ready...",0.3,0.6);
            break;
        case GAME_READY:
            display_text((char*)"Just be ready...",0.3,0.6);
            display_text((char*)"Waiting for other players...",0.3,0.5);
            display_text((char*)"Game can start any time...",0.3,0.4);
            break;
        case GAME_STARTED:
            char str[20];
            for(int i=0;i<4;i++)
            {
                if(gstate.wall_no==i)
                    sprintf(str,"Your Life: %d",gstate.paddle[gstate.wall_no].life);
                else
                    sprintf(str,"Paddle %d Life: %d",i+1,gstate.paddle[i].life);
                display_text(str,0.3,0.7-0.1*i);
            }
            break;
        case GAME_PAUSED:
            display_text((char*)"GAME PAUSED!",0.3,0.6);
            display_text((char*)"Press <space> when ready...",0.3,0.5);
            break;
        case GAME_FINISHED:
            if(gstate.winner!=-1)
            {
                if(gstate.winner==gstate.wall_no)
                    display_text((char*)"You won! :)",0.3,0.6);
                else
                    display_text((char*)"You lose! :(",0.3,0.6);
            }
            else
            {
                display_text((char*)"Server quits!",0.3,0.6);
            }
            //exit(0);
            break;
    }
}
void ClientGUI::display_text(char *str,float x,float y)
{
    int i;
    glColor3f(1,1,1);
    glRasterPos3f(x, y,0);
    for (i=0; str[i]!= '\0';i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
}
void init (void) 
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}
void processNormalKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            client.quitGame();
            break;
        case ' ':
            switch(gstate.status)
            {
                case GAME_READY:
                case GAME_STARTED:
                    //	  cout<<"Pausing game...\n";
                    client.pauseGame();
                    break;
                case GAME_WAITING:
                case GAME_PAUSED:
                    //	  cout<<"Starting game...\n";
                    client.startGame();
            }
            break;
    }
}
void processSpecialKeys(int key, int x, int y)
{

    switch(key)
    {
        case GLUT_KEY_LEFT:
            gstate.movePaddle(-1);
            client.sendMessage(POSITION);
            //		  cout<<"Left\n";
            break;
        case GLUT_KEY_RIGHT:
            gstate.movePaddle(1);
            client.sendMessage(POSITION);
            //		  cout<<"Right\n";
            break;
    }
}
ClientGUI cgui;
void display()
{
    cgui.display();
}
void* cgui_main(void*)
{
    int argc=1;
    char *argv[]={(char*)"client",NULL};
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Ping Pong");
    init ();
    glutDisplayFunc(display); 
    glutIdleFunc(display);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();
}


