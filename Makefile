all:
	g++ -ggdb blah.cc $$(pkg-config opencv --libs --cflags)
