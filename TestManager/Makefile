PROJECT = TM
HEADERS = $(PROJECT).h
OBJECTS = TM.o user_login.o socket_login.o tm_server.o tm_client.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJECTS)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJECTS)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

user_login.o : user_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c user_login.c

socket_login.o : socket_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c socket_login.c

tm_server.o : tm_server.c $(HEADERS)
		$(C11) $(CFLAGS) -c tm_server.c

tm_client.o : tm_client.c $(HEADERS)
		$(C11) $(CFLAGS) -c tm_client.c

clean:
	rm -f $(PROJECT) $(OBJECTS)