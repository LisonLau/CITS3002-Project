PROJECT = TM
HEADERS = $(PROJECT).h
OBJ = TM.o TM_web.o TM_QB.o user.o ques.o html.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
		$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

TM.o : TM.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM.c

TM_web.o : TM_web.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM_web.c

TM_QB.o : TM_QB.c $(HEADERS)
		$(C11) $(CFLAGS) -c TM_QB.c

user.o : user.c $(HEADERS)
		$(C11) $(CFLAGS) -c user.c

ques.o : ques.c $(HEADERS)
		$(C11) $(CFLAGS) -c ques.c

html.o : html.c $(HEADERS)
		$(C11) $(CFLAGS) -c html.c

clean:
	rm -f $(PROJECT) $(OBJ)