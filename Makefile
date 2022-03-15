
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall -g

all: correctness persistence test

test: TableUnitTest TableLevelTest

TableLevelTest: TableLevelTest.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

TableUnitTest: TableUnitTest.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

correctness: kvstore.o correctness.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

persistence: kvstore.o persistence.o skipList.o bloomfilter.o ssTableUnit.o ssTableHeader.o ssTableLevel.o ssTable.o

clean:
	-rm -f correctness persistence *.o

cleandata:
	rm -rf data/*
