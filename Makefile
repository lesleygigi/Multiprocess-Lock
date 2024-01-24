CC=gcc
CFLAGS=-std=gnu99 -Wall -pthread

all: libcounter.so liblist.so libhash.so libmq.so

clean: clean_counter clean_list clean_hash clean_mq

libcounter.so: counter.c spinlock.c mutex.c
	$(CC) $(CFLAGS) -shared -o $@ $^

liblist.so: list.c spinlock.c mutex.c
	$(CC) -Wall -pthread -shared -o $@ $^

libhash.so: hash.c list.c spinlock.c mutex.c
	$(CC) -Wall -pthread -shared -o $@ $^

libmq.so: mq.c
	$(CC) $(CFLAGS) -pthread -shared -o $@ $^

counter_test: counter_test.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lcounter -Wl,-rpath=.

list_test: list_test.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -llist -Wl,-rpath=.

hash_test: hash_test.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lhash -Wl,-rpath=.

mq_test: mq_test.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lmq -Wl,-rpath=.

performance: performance.c
	$(CC) -Wall -pthread -o $@ $^ -L. -lhash -Wl,-rpath=.

fairness: fairness.c
	$(CC) -Wall -pthread -o $@ $^ -L. -lcounter -Wl,-rpath=.

oneTypeOpera: oneTypeOpera.c
	$(CC) -Wall -pthread -o $@ $^ -L. -lhash -Wl,-rpath=.

both:both.c 
	$(CC) -Wall -pthread -o $@ $^ -L. -lhash -Wl,-rpath=.

scaling:scaling.c 
	$(CC) -Wall -pthread -o $@ $^ -L. -lhash -Wl,-rpath=.

clean_counter:
	rm -f counter_test
	rm -f libcounter.so
	rm -f fairness
clean_list:
	rm -f list_test
	rm -f liblist.so
clean_hash:
	rm -f hash_test
	rm -f libhash.so
	rm -f performance
	rm -f oneTypeOpera
	rm -f both
	rm -f scaling
clean_mq:
	rm -f mq_test
	rm -f libmq.so
	
	