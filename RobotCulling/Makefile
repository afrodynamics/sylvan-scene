NAME = water
ECHO = @echo
CC = @clang++
LIBS = -framework GLUT -framework OpenGL -framework Cocoa
LDFLAGS = $(LIBS)
CFLAGS = -g -std=c++11
SOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))
.SUFFIXES: .cpp .h .o

.cpp.o:
	$(ECHO) "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(ECHO) "Linking $@..."
	$(CC) -o $@ *.o $(LIBS)
	$(ECHO) "Built $@!"

clean:
	$(RM) core *.o $(NAME)
	$(ECHO) "All clean!"

new:
	make clean
	make
