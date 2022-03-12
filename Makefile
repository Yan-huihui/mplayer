
OBJ=a.out

SRC+=main.c
SRC+=ui.c
SRC+=cmd.c

$(OBJ):$(SRC)
	gcc $^ -o $@

.PHONY:
clear:
	rm $(OBJ)
