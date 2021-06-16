CXX= g++
CXXFLAGS= -Wall -Wextra -Wextra -pedantic -lm -std=c++11 -g
SRC= main.cc file_info.cc file_update.cc file_injection.cc handler.cc
OBJ=${SRC:.cc=.o}	
LDLIBS= `pkg-config gtk+-2.0 --cflags --libs`
EXEC= main
all: ${OBJ} 
	${CXX} -o ${EXEC} ${OBJ} ${CXXFLAGS} ${LDLIBS}

%.o: %.cc
	${CXX} $< -c -o $@ ${CXXFLAGS} ${LDLIBS}
.PHONY: all clean

clean:
	${RM} ${EXEC}
	${RM} ${OBJ}
