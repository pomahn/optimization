all:
	gcc -O0 main.c my_hashtable.c -o unoptimized
	gcc -O0 main.c my_hash_opt.c -o optimized

