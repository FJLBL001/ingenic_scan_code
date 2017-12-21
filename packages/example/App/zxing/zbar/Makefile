# Project: Zbar
# Makefile created by Dev-C++ 5.11

CPP      = mips-linux-gnu-g++
CC       = mips-linux-gnu-gcc
AR       = mips-linux-gnu-ar

OBJ      = decoder/code39.o decoder/code128.o decoder/ean.o decoder/i25.o decoder/pdf417.o decoder/qr_finder.o qrcode/bch15_5.o qrcode/binarize.o qrcode/isaac.o qrcode/qrdec.o qrcode/qrdectxt.o qrcode/rs.o qrcode/util.o config.o decoder.o error.o image.o img_scanner.o refcnt.o scanner.o symbol.o  decoder/codabar.o decoder/databar.o decoder/code93.o
LINKOBJ  = decoder/code39.o decoder/code128.o decoder/ean.o decoder/i25.o decoder/pdf417.o decoder/qr_finder.o qrcode/bch15_5.o qrcode/binarize.o qrcode/isaac.o qrcode/qrdec.o qrcode/qrdectxt.o qrcode/rs.o qrcode/util.o config.o decoder.o error.o image.o img_scanner.o refcnt.o scanner.o symbol.o decoder/codabar.o decoder/databar.o decoder/code93.o
LIBS     = 
INCS     = -I "./" -I"./decoder/" -I"./qrcode/"
CXXINCS  = -I "./" -I"./decoder/" -I"./qrcode/"
BIN      = libzbar.a
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS) 
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(AR) -r $(BIN) $(LINKOBJ)  $(LIBS)

decoder/code39.o: decoder/code39.c
	$(CC) -c decoder/code39.c -o decoder/code39.o $(CFLAGS)

decoder/code128.o: decoder/code128.c
	$(CC) -c decoder/code128.c -o decoder/code128.o $(CFLAGS)

decoder/ean.o: decoder/ean.c
	$(CC) -c decoder/ean.c -o decoder/ean.o $(CFLAGS)

decoder/i25.o: decoder/i25.c
	$(CC) -c decoder/i25.c -o decoder/i25.o $(CFLAGS)

decoder/pdf417.o: decoder/pdf417.c
	$(CC) -c decoder/pdf417.c -o decoder/pdf417.o $(CFLAGS)

decoder/qr_finder.o: decoder/qr_finder.c
	$(CC) -c decoder/qr_finder.c -o decoder/qr_finder.o $(CFLAGS)

qrcode/bch15_5.o: qrcode/bch15_5.c
	$(CC) -c qrcode/bch15_5.c -o qrcode/bch15_5.o $(CFLAGS)

qrcode/binarize.o: qrcode/binarize.c
	$(CC) -c qrcode/binarize.c -o qrcode/binarize.o $(CFLAGS)

qrcode/isaac.o: qrcode/isaac.c
	$(CC) -c qrcode/isaac.c -o qrcode/isaac.o $(CFLAGS)

qrcode/qrdec.o: qrcode/qrdec.c
	$(CC) -c qrcode/qrdec.c -o qrcode/qrdec.o $(CFLAGS)

qrcode/qrdectxt.o: qrcode/qrdectxt.c
	$(CC) -c qrcode/qrdectxt.c -o qrcode/qrdectxt.o $(CFLAGS)

qrcode/rs.o: qrcode/rs.c
	$(CC) -c qrcode/rs.c -o qrcode/rs.o $(CFLAGS)

qrcode/util.o: qrcode/util.c
	$(CC) -c qrcode/util.c -o qrcode/util.o $(CFLAGS)

config.o: config.c
	$(CC) -c config.c -o config.o $(CFLAGS)

decoder.o: decoder.c
	$(CC) -c decoder.c -o decoder.o $(CFLAGS)

error.o: error.c
	$(CC) -c error.c -o error.o $(CFLAGS)

image.o: image.c
	$(CC) -c image.c -o image.o $(CFLAGS)

img_scanner.o: img_scanner.c
	$(CC) -c img_scanner.c -o img_scanner.o $(CFLAGS)

refcnt.o: refcnt.c
	$(CC) -c refcnt.c -o refcnt.o $(CFLAGS)

scanner.o: scanner.c
	$(CC) -c scanner.c -o scanner.o $(CFLAGS)

symbol.o: symbol.c
	$(CC) -c symbol.c -o symbol.o $(CFLAGS)

decoder/codabar.o: decoder/codabar.c
	$(CC) -c decoder/codabar.c -o decoder/codabar.o $(CFLAGS)

decoder/databar.o: decoder/databar.c
	$(CC) -c decoder/databar.c -o decoder/databar.o $(CFLAGS)

decoder/code93.o: decoder/code93.c
	$(CC) -c decoder/code93.c -o decoder/code93.o $(CFLAGS)

