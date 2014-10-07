FILES=common.c tsc.c

tracker: tracker.c $(FILES)
	gcc -Wall tracker.c $(FILES) -o tracker
	
forker: forker.c $(FILES)
	gcc -Wall forker.c $(FILES) -o forker
