src = $(wildcard src/*.c)
obj = $(patsubst src%,out%,$(src:.c=.o))
dep = $(obj:.o=.d))
exe = bin/babble

override CFLAGS = -Ivendor/curl/include \
				  -Ivendor/libuwsc/src -Ivendor/libuwsc/src/buffer \
				  -Ivendor/libev
LDFLAGS = -Lvendor/curl/lib/.libs -l:libcurl.a \
		  -Lvendor/libev/.libs -l:libev.a \
		  -Lvendor/libuwsc/build/src -l:libuwsc.a \
		  -pthread -ldl -lldap -llber -lz -lssl -lcrypto -lm

all: pre babble

pre:
	@mkdir -p bin
	@mkdir -p out

babble: $(obj) 
	@echo "Compiling babble..."
	$(CC) -o $(exe) $^ $(LDFLAGS)
	@echo "Done"

-include $(dep)

%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

out/%.o: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: vendor
vendor: curl libev libuwsc

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

.PHONY: clean
clean:
	rm -f bin/babble $(obj)

.PHONY: cleandep
cleandep:
	rm -f $(dep)
