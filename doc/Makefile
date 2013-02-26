CC = pdflatex

TARGET = projectplan.pdf

.PHONY: all clean cleanalll
all:  	$(TARGET)

%.pdf:	%.tex
	for nr in 1 2 3 ; do\
		$(CC) $^ ; \
	done

clean:
	rm -f *.aux *.log

cleanall:
	rm -f $(TARGET) *.aux *.log
