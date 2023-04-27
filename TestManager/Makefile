PROJECT = TM
HEADERS = $(PROJECT).h
OBJ = TM.o user.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

user.o : user.c $(HEADERS)
		$(C11) $(CFLAGS) -c user.c
