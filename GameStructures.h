#ifndef GAME_STRUCTURES
#define GAME_STRUCTURES

//#include <sys/socket.h>
#include <netinet/in.h>
//#include <arpa/inet.h>

#define MAX_BALLS 3

#define BALL_SIZE 0.03
#define PADDLE_LENGTH 0.3
#define PADDLE_WIDTH 0.03
#define L (1-2*PADDLE_WIDTH)


typedef struct
{
  float x,y;
} Vector;

enum PlayerType{AUTO,HUMAN,COMPUTER};

enum PlayerState{PLAYER_NA,PLAYER_CONNECTED,
		 PLAYER_READY,PLAYER_PLAYING,
		 PLAYER_PAUSED,PLAYER_FINISHED};

enum Command{TEST,CONNECT,INIT,READY,
	     START,POSITION,PAUSE,QUIT};

struct Paddle
{
  PlayerType ptype;
  PlayerState pstate;
  float position;/*valid, if open*/
  int life;
};

struct Ball
{
  Vector position;
  Vector velocity;
};


struct PlayerInfo
{
  sockaddr_in client_addr;
  timeval last_msg_time;
  int last_pkt_num;//incoming
  int serv_pkt_num;//outgoing
  int total_pkts_lost;//incoming
//  Command last_cmd;
};


struct ServerMessage//communicated state
{
  Command command;
  int pkt_num;

  /*one time*/
  int wall_no;
  int num_balls;
  int winner;

  /*usual*/
  Paddle paddle[4];
  Ball ball[MAX_BALLS];
};
struct ClientMessage//communicated state
{
  Command command;
  int pkt_num;
  float paddle_position;
//  PlayerState pstate;
};

#endif
