all:
	g++ src/main.cpp src/game.cpp src/AI.cpp src/types.cpp src/queue.cpp -o singleplayer -lm
	g++ src/server.cpp src/game.cpp src/AI.cpp src/types.cpp src/queue.cpp -o server -lm -lpthread
	g++ src/client.cpp src/game.cpp src/AI.cpp src/types.cpp src/queue.cpp -o client -lm -lpthread
