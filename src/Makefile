CXXFLAGS =	-O2 -g -Wall 

OBJS =		readnl.o

LIBS =

TARGET =	readnl

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
