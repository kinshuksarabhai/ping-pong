#define MAX_BALLS 3

#define BALL_SIZE 0.07
#define PADDLE_LENGTH 0.3
#define PADDLE_WIDTH 0.05
#define L (1-2*PADDLE_WIDTH)


typedef struct
{
  float x,y;
} Vector;

enum PlayerState{PLAYER_NA,PLAYER_CONNECTED,PLAYER_READY,PLAYER_PLAYING,PLAYER_FINISHED};
struct Paddle
{
  PlayerState pstate; 
  float position;/*valid, if open*/
  sockaddr_in client_addr;
};

struct Ball
{
  Vector position;
  Vector velocity;
};

enum Command{CONNECT,INIT,READY,START,POSITION,PAUSE,QUIT};
struct ServerMessage
{
  Command command;
  /*one time*/
  int wall_no;
  int num_balls;
  /*usual*/
  Paddle paddle[4];
  Ball ball[MAX_BALLS];
};
struct ClientMessage
{
  Command command;
  int wall_no;//who is sending??
  float paddle_position;
};
