CC = pdflatex

COMPILE_RUNS = 1 2 3
TARGET = projectplan.pdf

.PHONY: all clean cleanalll
all:  	$(TARGET)

%.pdf:	%.tex
	$(foreach NB, $(COMPILE_RUNS), $(CC) $^;)

clean:
	rm -f *.aux *.log

cleanall:
	rm -f $(TARGET) *.aux *.log
