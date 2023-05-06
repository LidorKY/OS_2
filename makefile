all:cmp copy encode decode stshell

stshell:stshell.o
	gcc -Wall -g -o stshell stshell.o

stshell.o:stshell.c
	gcc -Wall -g -c stshell.c

cmp:cmp.o
	gcc -Wall -g -o cmp cmp.o

cmp.o:cmp.c
	gcc -Wall -g -c cmp.c

copy:copy.o
	gcc -Wall -g -o copy copy.o

copy.o:copy.c
	gcc -Wall -g -c copy.c

encode: encode.o libcodecA.so libcodecB.so
	gcc -Wall -g -ldl -o encode encode.o

decode: decode.o libcodecA.so libcodecB.so
	gcc -Wall -g -ldl -o decode decode.o

encode.o: encode.c
	gcc -Wall -g -c encode.c

decode.o: decode.c
	gcc -Wall -g -c decode.c

libcodecA.so:codecA.o
	gcc -Wall -g -shared -o libcodecA.so codecA.o

libcodecB.so:codecB.o
	gcc -Wall -g -shared -o libcodecB.so codecB.o

codecA.o:codecA.c
	gcc -Wall -g -fPIC -c codecA.c

codecB.o:codecB.c
	gcc -Wall -g -fPIC -c codecB.c

clean:
	rm -f *.o *.so cmp copy output encode decode stshell