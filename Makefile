CC=gcc
LIBS=-lgmp

OBJ = encode.c

%.o: %.c
	$(CC) -c -o $@

encode: $(OBJ)
	gcc -o $@ $^ $(LIBS)
