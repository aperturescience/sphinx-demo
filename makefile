all:
	gcc -o demo demo.c \
  	-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" \
  	`pkg-config --cflags --libs pocketsphinx sphinxbase` \

clean:
	rm -rf demo