mp3: mp3main.c serialize.c
	gcc -ggdb3 mp3main.c serialize.c -o mp3
clean:
	rm -f mp3 *~
