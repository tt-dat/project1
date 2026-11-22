CC = gcc

INCDIR = ./include
CFLAGS = -I$(INCDIR)

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin

# get .c file in src
SRCS = $(wildcard $(SRCDIR)/*.c)

# get .o file in obj
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))


TARGET = sniff


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)


clean:
	rm -f $(OBJS) $(TARGET)


test: 
	echo $(OBJS)
