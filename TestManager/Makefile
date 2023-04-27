PROJECT = TM
HEADERS = $(PROJECT).h
OBJ = TM.o user_login.o socket_login.o TM_server.o TM_client.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

TM_server.o : TM_server.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM_server.c

TM_client.o : TM_client.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM_client.c

user_login.o : user_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c user_login.c

socket_login.o : socket_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c socket_login.c