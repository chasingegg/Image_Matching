CC=			g++
CFLAGS=		-c -g -O0 -Wall `pkg-config --cflags opencv libconfig++`
LDFLAGS=	`pkg-config --libs opencv libconfig++`

SOURCESCOM=	Common/Image.cpp Features/FeatureHandler.cpp
SOURCESOBJ=	ImageMatch.cpp

OBJECTSCOM=	$(SOURCESCOM:.cpp=.o)
OBJECTSOBJ=	$(SOURCESOBJ:.cpp=.o)

all:  FastVersion
	
FastVersion: $(OBJECTSCOM) $(OBJECTSOBJ)
	$(CC) $(OBJECTSCOM) $(OBJECTSOBJ) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTSCOM) $(OBJECTSTST) $(OBJECTSCNI) $(OBJECTSFVD) $(OBJECTSOBJ) \
	demo
