CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lmysqlclient -lpthread

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGETS = server client

all: $(TARGETS)

server: src/server.cpp src/auth.cpp src/dbconnect.cpp src/loadMsg.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

client: src/client.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

clean:
	rm -f bin/* src/*.o
