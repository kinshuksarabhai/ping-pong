class ClientGUI
{
public:
  void display();
  void display_board();
  void display_paddles();
  void display_ball(int);
};
void ClientGUI::display()
{
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
    case 27:cout<<"escape\n";
      client.sendMessage(QUIT);
      exit(0);
      break;
    case ' ':cout<<"space\n";
      if(gstate.status==GAME_STARTED)
	{
	  cout<<"Pausing game...\n";
	  gstate.status=GAME_PAUSED;
	client.sendMessage(PAUSE);
	}
      else if (gstate.status==GAME_PAUSED || gstate.status==GAME_WAITING)
	{
	  cout<<"Started game...\n";
	  gstate.status=GAME_STARTED;
	client.sendMessage(READY);
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
		  cout<<"Left\n";
		  break;
		case GLUT_KEY_RIGHT:
		  gstate.movePaddle(1);
		  cout<<"Right\n";
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
  char *argv[]={"client",NULL};
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


