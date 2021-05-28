CCX= g++
CXXFLAGS= -Wall -Wextra -Wextra -pedantic -std=c++11 -g
SRC= handler.cc
OBJ=${SRC:.cc=.o}	
EXEC= main
all: ${OBJ}
	${CXX} -o ${EXEC} ${OBJ} ${CXXFLAGS}

.PHONY: all clean

clean:
	${RM} ${EXEC}
	${RM} ${OBJ}
