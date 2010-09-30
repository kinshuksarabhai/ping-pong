#define MAX_BALLS 3
typedef struct
{
  float x,y;
} vector;

enum {BRICKED,OPEN};
typedef struct
{
  int state;
  int player_id;
} wall;

typedef struct
{
  vector position,velocity;
} ball;

typedef struct
{
  int player_id;
  int wall_no;
  socket sockfd;
  int state;
} player;

vector board_size;
float paddle_length;
int num_balls;
int ball_size;
int hardness_level;

int game_state;

wall side[4];
ball num_balls[MAX_BALLS];

void initStructures()
{
board_size
}
