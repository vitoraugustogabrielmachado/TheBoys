# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99
LDLIBS  = -lm
MAIN    = theboys
ENTREGA = $(MAIN)

# lista de arquivos de cabeçalho
HDR = fila.h fprio.h conjunto.h eventos.h entidades.h

# lista de arquivos-objeto (não inclua conjunto.o)
OBJ = fila.o fprio.o theboys.o eventos.o entidades.o

# construir o executável
$(MAIN): $(OBJ) conjunto.o
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ) conjunto.o $(LDLIBS)

# construir o main
theboys.o: theboys.c $(HDR)

# construir os TADs
fila.o: fila.c fila.h
	$(CC) $(CFLAGS) -c fila.c

fprio.o: fprio.c fprio.h
	$(CC) $(CFLAGS) -c fprio.c

eventos.o: eventos.c eventos.h
	$(CC) $(CFLAGS) -c eventos.c

entidades.o: entidades.c entidades.h
	$(CC) $(CFLAGS) -c entidades.c



# executar
run: $(MAIN)
	./$(MAIN)

# testar no Valgrind
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

# gerar arquivo TGZ para entrega
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp *.c *.h conjunto.o makefile /tmp/$(USER)/$(ENTREGA)
	tar czvf $(ENTREGA).tgz -C /tmp/$(USER) $(ENTREGA)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

# limpar arquivos temporários
clean:
	rm -f *~ $(OBJ) $(MAIN) /tmp/$(USER)/$(ENTREGA) $(ENTREGA).tgz
