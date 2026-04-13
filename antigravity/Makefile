CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRCS = main.c \
       lot1_joueur/joueur.c \
       lot2_background/background.c \
       lot3_npc/npc.c \
       lot4_minimap/minimap.c \
       lot5_enigme1/enigme1.c

OBJS = $(SRCS:.c=.o)
TARGET = antigravity

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
