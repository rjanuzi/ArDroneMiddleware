all:
	gcc -c *.c -I.
	gcc -o main.out *.o -lpthread
	rm *.o

clean:
	rm main.out
