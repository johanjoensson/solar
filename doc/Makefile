CC = pdflatex

TARGET = projectplan.pdf

.PHONY: all clean cleanalll
all:  	$(TARGET)

%.pdf:	%.tex
	$(CC) $^

clean:
	rm -f *.aux *.log

cleanall:
	rm -f $(TARGET) *.aux *.log
