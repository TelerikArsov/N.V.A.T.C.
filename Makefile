all:
	g++ blah.cc $$(pkg-config opencv --libs --cflags)
