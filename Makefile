
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall

all: correctness persistence

correctness: kvstore.o correctness.o skipList.o

persistence: kvstore.o persistence.o skipList.o

clean:
	-rm -f correctness persistence *.o
