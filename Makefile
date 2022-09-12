CC      = gcc     #default compiler
CFLAGS  = -g      #compilation flags
TARGET  = driver
OBJS    = driver.o loglib.o
.SUFFIXES: .c .o

.DEFAULT_GOAL : $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	/bin/rm -f *.o $(TARGET)
