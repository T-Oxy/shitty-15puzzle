TARGET=15puzzle

$(TARGET): main.o ImageData.o ImageIO.o GenericTimer.o
	cc -o $(TARGET) main.o ImageData.o ImageIO.o GenericTimer.o -L. -lglut -lGLU -lGL -lXi -lXrandr -lm
.c.o:
	cc -c $< -I. -Wall
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f *.o *.c~ *.h~ $(TARGET)
