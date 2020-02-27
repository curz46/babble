#!make

src = $(wildcard src/*.c)
obj = $(patsubst src%,out%,$(src:.c=.o))
exe = bin/babble

envfile = .env
include $(envfile)

INCLUDE = -Iinclude \
		  -Ivendor/curl/include \
		  -Ivendor/libuwsc/src -Ivendor/libuwsc/src/buffer \
		  -Ivendor/libev \
		  -Ivendor/jansson/src
LDFLAGS = -Lvendor/curl/lib/.libs -l:libcurl.a \
		  -Lvendor/libev/.libs -l:libev.a \
		  -Lvendor/libuwsc/build/src -l:libuwsc.a \
		  -Lvendor/jansson/src/.libs -l:libjansson.a \
		  -pthread -ldl -lldap -llber -lz -lssl -lcrypto -lm -lnghttp2 -lidn2 -lrtmp -lpsl -lbrotlidec
SOURCE_EXT =

.PHONY: all
all: pre $(exe)

.PHONY: pre
pre:
	@mkdir -p bin
	@mkdir -p out

$(exe): $(obj)
	@echo "Compiling babble..."
	gcc $(CFLAGS) -o $(exe) $(SOURCE_EXT) $^ $(LDFLAGS)
	@echo "Done"

out/%.o: src/%.c
	gcc -g3 $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c -o $@ $<

.PHONY: test
test: $(exe)
	TOKEN=$(TOKEN) ./bin/babble

.PHONY: vendor
vendor: curl libev libuwsc jansson

.PHONY: curl
curl:
	cd vendor/curl && ./buildconf
	cd vendor/curl && ./configure
	cd vendor/curl && make

.PHONY: libev
libev:
	cd vendor/libev && chmod +x ./autogen.sh
	cd vendor/libev && ./autogen.sh
	cd vendor/libev && ./configure
	cd vendor/libev && make

.PHONY: libuwsc
libuwsc:
	cd vendor/libuwsc && mkdir -p build
	cd vendor/libuwsc/build && CMAKE_INCLUDE_PATH="../../libev" CMAKE_LIBRARY_PATH="../../libev/.libs" cmake ..
	cd vendor/libuwsc/build && make

.PHONY: jansson
jansson:
	cd vendor/jansson && autoreconf -i
	cd vendor/jansson && ./configure
	cd vendor/jansson && make

.PHONY: clean
clean:
	rm -f bin/babble $(obj)
