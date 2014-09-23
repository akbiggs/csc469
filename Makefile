FILES=tracker.c tsc.c

tracker: $(FILES)
	gcc -Wall $(FILES)
