CC = g++
CFLAGS = `pkg-config --cflags opencv`
LDLIBS = `pkg-config --libs opencv`
TARGET = ex_2_1.cpp


a.out: $(TARGET)
	$(CC) -o a.out $(TARGET) $(CFLAGS) $(LDLIBS)
