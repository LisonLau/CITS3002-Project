PROJECT = TM
HEADERS = $(PROJECT).h
OBJ = TM.o user_login.o socket_login.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

user_login.o : user_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c user_login.c

socket_login.o : socket_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c socket_login.c