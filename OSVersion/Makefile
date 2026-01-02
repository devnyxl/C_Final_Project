CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = attendance_system
OBJ = main.o lecturer.o academic.o student.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c system.h
	$(CC) $(CFLAGS) -c main.c

lecturer.o: lecturer.c system.h
	$(CC) $(CFLAGS) -c lecturer.c

academic.o: academic.c system.h
	$(CC) $(CFLAGS) -c academic.c

student.o: student.c system.h
	$(CC) $(CFLAGS) -c student.c

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run