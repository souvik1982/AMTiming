all: AMTiming

clean:
	rm *.o

AMTiming: Component.o ComponentRelation.o DataSource.o StubMapper.o AssociativeMemory.o HitBuffer.o AMTiming.o
	c++ Component.o ComponentRelation.o DataSource.o StubMapper.o AMTiming.o AssociativeMemory.o HitBuffer.o -o AMTiming

DataSource.o: src/DataSource.cc
	c++ -c src/DataSource.cc

StubMapper.o: src/StubMapper.cc
	c++ -c src/StubMapper.cc

AssociativeMemory.o: src/AssociativeMemory.cc
	c++ -c src/AssociativeMemory.cc

HitBuffer.o: src/HitBuffer.cc
	c++ -c src/HitBuffer.cc

ComponentRelation.o: src/ComponentRelation.cc
	c++ -c src/ComponentRelation.cc

Component.o: src/Component.cc
	c++ -c src/Component.cc

