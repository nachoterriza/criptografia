CC=gcc
LIBS=-lgmp
LIBS2=-lm

OBJ = encode.c
OBJ2 = rsa.c
OBJ3 = rsa2.c

%.o: %.c
	$(CC) -c -o $@

encode: $(OBJ)
	gcc -o $@ $^ $(LIBS)

rsa: $(OBJ2)
	gcc -o $@ $^ $(LIBS2)

rsa2: $(OBJ3)
	gcc -o $@ $^ $(LIBS2)

clean:
	 rm -f encode rsa rsa2
