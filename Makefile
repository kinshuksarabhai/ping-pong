all: server client

client: false
	g++ -g -o client -lm -lGL -lGLU -lglut GameClient.cpp

server: false
	g++ -g -o server -lm -lpthread GameServer.cpp

false: