all: pcap-test

pcap-test: main.o pcap-test.o
	g++ -o pcap-test main.o pcap-test.o -lpcap
	rm *.o
pcap-test.o: pcap-test.cpp pcap-test.h
	g++ -c -o pcap-test.o pcap-test.cpp

main.o: main.cpp
	g++ -c -o main.o main.cpp

clean:
	rm -f pcap-test

