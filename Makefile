all: server_target client_target

client_target: GameClient.cpp NetworkClient.cpp ClientGUI.cpp GameState.cpp GameStructures.h
	g++ -g -o client GameClient.cpp -lm -lGL -lGLU -lglut 

server_target: GameServer.cpp NetworkServer.cpp GameState.cpp GameStructures.h
	g++ -g -o server GameServer.cpp -lm -lpthread 

client: .fake
	g++ -g -o client client.cpp ClientGUI.o GameState.o NetworkClient.o -lm -lGL -lGLU -lglut -lpthread

client_gui: .fake
	g++ -g -c ClientGUI.cpp

game_state: .fake
	g++ -g -c GameState.cpp

network_client: .fake
	g++ -g -c NetworkClient.cpp

.fake:

false:

run: all false
	./server 3000 1 1 0.3 &
	./client
	@ killall server

game:
	./game

run-2player: all false
	./server 3000 1 1 0.3 &
	./client &
	./client
	@ killall server

run-network-2: all false
#	PING-PONG-REMOTE-SERVER = localhost
#	ssh siy107536@palasi.cse.iitd.ac.in
	./server 3000 2 1 &
	./client &
	./client
	@ killall server

#automated input test
test1: all false
	./server 3000 1 1 0.3 &
	./client &
	./tester.sh 
	@ killall server
	echo "Test Pass"

#simulated network delay test
test2: false
	sudo tc qdisc add dev lo root netem delay 100ms
	make run
	sudo tc qdisc del dev lo root

#simulated network losses test
test3: false
	sudo tc qdisc add dev lo root netem loss 60%
	make run
	sudo tc qdisc del dev lo root

#simulated network packet duplication test
test4: false
	sudo tc qdisc add dev lo root netem duplicate 1%
	make run
	sudo tc qdisc del dev lo root

