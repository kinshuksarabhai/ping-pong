#define MAX_BALLS 3

#define BALL_SIZE 0.07
#define PADDLE_LENGTH 0.3
#define PADDLE_WIDTH 0.05
#define L (1-2*PADDLE_WIDTH)


typedef struct
{
  float x,y;
} Vector;

enum State{BRICKED,OPEN};

typedef struct
{
  State state;
  float position;/*valid, if open*/
} Paddle;

typedef struct
{
  Vector position;
  Vector velocity;
} Ball;
