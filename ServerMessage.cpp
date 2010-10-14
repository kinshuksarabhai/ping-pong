struct ServerInitMessage
{
  int wall_no;
};
struct ServerUpdateMessage
{
  Paddle paddle[4];
  Ball ball[MAX_BALLS];
};
