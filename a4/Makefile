CFLAGS += -std=gnu11
EXES   =  stack a1
OBJS   =  stack.o a1.o list.o tree.o

all: $(EXES)

stack: stack.o
a1: a1.o list.o tree.o

clean:
	rm -f $(EXES) $(OBJS)
tidy:
	rm -f $(OBJS)