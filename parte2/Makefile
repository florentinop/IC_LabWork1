all:
	@(if [ ! -d build ]; then mkdir build; fi; cd build;\
	   cmake ..; make)

clean:
	rm -rf BitStream
	rm -rf Decoder
	rm -rf Encoder
	rm -r build
