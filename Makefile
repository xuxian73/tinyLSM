
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall

all: correctness persistence

correctness: kvstore.o correctness.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

persistence: kvstore.o persistence.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

clean:
	-rm -f correctness persistence *.o
