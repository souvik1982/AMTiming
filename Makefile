ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: AMTiming

clean:
	rm *.o

AMTiming: RootDictionary.o EventCharacteristics.o Component.o ComponentRelation.o DataSource.o StubMapper.o AssociativeMemory.o HitBuffer.o CombinationBuilder.o TrackFitter.o AMTiming.o
	c++ RootDictionary.o EventCharacteristics.o Component.o ComponentRelation.o DataSource.o StubMapper.o AMTiming.o AssociativeMemory.o HitBuffer.o CombinationBuilder.o TrackFitter.o -o AMTiming $(ROOTFLAGS) $(ROOTLIBS)

RootDictionary.o: RootDictionary.cc
	c++ -c RootDictionary.cc -c $(ROOTFLAGS)

EventCharacteristics.o: src/EventCharacteristics.cc
	c++ -c src/EventCharacteristics.cc -c $(ROOTFLAGS)

DataSource.o: src/DataSource.cc
	c++ -c src/DataSource.cc -c $(ROOTFLAGS)

StubMapper.o: src/StubMapper.cc
	c++ -c src/StubMapper.cc -c $(ROOTFLAGS)

AssociativeMemory.o: src/AssociativeMemory.cc
	c++ -c src/AssociativeMemory.cc -c $(ROOTFLAGS)

HitBuffer.o: src/HitBuffer.cc
	c++ -c src/HitBuffer.cc -c $(ROOTFLAGS)

CombinationBuilder.o: src/CombinationBuilder.cc
	c++ -c src/CombinationBuilder.cc -c $(ROOTFLAGS)

TrackFitter.o: src/TrackFitter.cc
	c++ -c src/TrackFitter.cc -c $(ROOTFLAGS)

ComponentRelation.o: src/ComponentRelation.cc
	c++ -c src/ComponentRelation.cc -c $(ROOTFLAGS)

Component.o: src/Component.cc
	c++ -c src/Component.cc -c $(ROOTFLAGS)

AMTiming.o: AMTiming.cc
	c++ -c AMTiming.cc -c $(ROOTFLAGS) 
