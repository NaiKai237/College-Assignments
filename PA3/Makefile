all: webproxy.o cache

cache:
	mkdir cache

webproxy.o: iabe3069_proxy.c
	gcc iabe3069_proxy.c -o webproxy -lcurl

run: webproxy.o cache
	./webproxy 8080

clean: 
	rm webproxy;rm -r ./cache
