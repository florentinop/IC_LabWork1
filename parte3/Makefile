all:
	@(if [ ! -d build ]; then mkdir build; fi; cd build;\
	   cmake ..; make)

clean:
	rm -rf wav_decoder
	rm -rf wav_encoder
	rm -r build
