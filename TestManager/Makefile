PROJECT = TM
HEADERS = $(PROJECT).h
OBJ = TM.o registered.o authenticate.o socket_login.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

registered.o : registered.c $(HEADERS)
		$(C11) $(CFLAGS) -c registered.c

authenticate.o : authenticate.c $(HEADERS)
		$(C11) $(CFLAGS) -c authenticate.c

socket_login.o : socket_login.c $(HEADERS)
		$(C11) $(CFLAGS) -c socket_login.c