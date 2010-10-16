all: false
	g++ -g -o client -lGL -lGLU -lglut GameClient.cpp
server: false
	g++ -g -o server -lpthread GameServer.cpp

false: