build:
	gcc autocomplete-movie-data-bst.c bst.c -o autocomplete-movie-data-bst -std=c11 -Wall
clean:
	rm autocomplete-movie-data-bst

run:
	clear
	./autocomplete-movie-data-bst
