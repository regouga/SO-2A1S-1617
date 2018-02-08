i-banco: i-banco.o contas.o commandlinereader.o
	gcc i-banco.o contas.o commandlinereader.o -o i-banco

i-banco.o: i-banco.c
	gcc -g -Wall -pedantic -c i-banco.c

contas.o: contas.c contas.h
	gcc -g -Wall -pedantic -c contas.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -g -Wall -pedantic -c commandlinereader.c

clean:
	rm *.o