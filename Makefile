
toolchain/arm-2007q3/.unpacked: downloads/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

downloads/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2:
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1793/public/arm-none-eabi/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2

clean:
	rm -f *~

clobber:
	rm -rf toolchain
