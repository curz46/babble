output: babble.o
	gcc build/babble.o -o out/babble

babble.o:
	gcc -c src/babble.c -o build/babble.o
