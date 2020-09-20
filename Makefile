# Author> Richard Fernando Heise Ferreira
# GRR20191053

GCCFLAGS = -Wall

BINARIES = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix

all: $(BINARIES)

# rules
wavinfo : wavinfo.o  wav.o 
wavvol  : wavvol.o	 wav.o
wavnorm : wavnorm.o  wav.o
wavrev  : wavrev.o   wav.o
wavecho : wavecho.o  wav.o
wavwide : wavwide.o  wav.o
wavcat  : wavcat.o   wav.o
wavmix  : wavmix.o   wav.o

wav.o: wav.c wav.h

clean:
	-rm -f *.o

purge: clean
	-rm -f $(BINARIES)
