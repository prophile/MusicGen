MusicGen.a: Channel.o Generator.o Group.o PRNG.o Sample.o
	rm -f $@ ; ar -qs $@ $^

clean:
	rm -f MusicGen.a *.o
