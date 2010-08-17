CC = g++
CFLAGS = -c -Wall -W -pthread -std=c++0x 
INCLUDEFLAGS = -Isrc/dis/stv/dec -Isrc/dis/stv/draw -Isrc/dis/stv/gen -Isrc/dis/stv/io -Isrc/dis/stv/main -I/usr/local/boost -I/usr/include/ImageMagick
LDFLAGS = -L/usr/lib -Wl,-Bsymbolic-functions -L/usr/lib/X11 -L/usr/lib -lMagick++ -lMagickWand -lMagickCore -llcms -ltiff -lfreetype -ljpeg -lXext -lSM -lICE -lX11 -lXt -lbz2 -lz -lm -lgomp -lpthread -lltdl
SOURCES = src/dis/stv/io/git_repository_access.cpp src/dis/stv/gen/space_colonizer.cpp src/dis/stv/draw/scanline_artist.cpp src/dis/stv/dec/spatial_displacement.cpp src/dis/stv/main/simple_tree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXAMPLEBINARY = bin/tree_gen_example
PROJECTBINARY = bin/simple_tree

all: $(SOURCES) $(PROJECTBINARY)

$(PROJECTBINARY): $(OBJECTS)
	$(CC) $(INCLUDEFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(LDFLAGS)  $< -o $@

clean:
	rm $(OBJECTS)
