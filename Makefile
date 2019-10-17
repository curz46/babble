output: babble.o
	@echo "Dependencies compiled."
	@echo "Compiling babble..."
	$(CC) build/babble.o vendor/curl/lib/*.o -pthread -ldl -lcurl -lldap -llber -lz -lssl -lcrypto -o out/babble
	@echo "Done"

vendor:
	@echo "Compiling dependencies..."
	cd ./vendor/curl && ./buildconf
	cd ./vendor/curl && ./configure
	@echo "Compiling libhttp..."
	$(MAKE) -C vendor/curl

babble.o:
	$(CC) -c src/babble.c -o build/babble.o -Isrc -Ivendor/curl/include
