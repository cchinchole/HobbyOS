# HobbyOS
This is my hobbyos that I am developing for fun through barebones.org and james molley's documentation.

## Building the CC

### Dependencies
build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev grub2 xorriso qemu-system-i386

### Setting PATH
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

### Binutils
https://ftp.gnu.org/gnu/binutils/

cd $HOME/src 
mkdir build-binutils
cd build-binutils
../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

### GCC
https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/

cd $HOME/src
mkdir build-gcc
cd build-gcc
../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

## Running
make run
