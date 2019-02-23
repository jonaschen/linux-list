
.PHONY: all clean
.DEFAULT_GOAL := all

TARGET = merge-sort
SOURCE = merge-sort.c

CFLAGS = -I../include -I../private

all:
	gcc $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)
