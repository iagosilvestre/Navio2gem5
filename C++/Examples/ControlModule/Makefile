CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) $(INCLUDES) controlM.cpp -L$(NAVIO) -lnavio -o controlM -static

clean:
	rm -f controlM
