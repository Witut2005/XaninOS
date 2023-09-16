#ORGINAL FILE  https://github.com/mell-o-tron/MellOs/blob/main/A_Setup/setup-gcc-arch.sh

#Installing dependencies

sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install python3 -y
sudo apt-get install python3-pip -y
sudo apt-get install make -y 
sudo apt-get install curl -y
sudo apt-get install nasm -y
sudo apt-get install qemu-system-x86 -y
sudo apt-get install virt-manager -y
sudo apt-get install virt-viewer -y
sudo apt-get install dnsmasq -y
sudo apt-get install vde2 -y
sudo apt-get install bridge-utils -y
sudo apt-get install bison -y
sudo apt-get install flex -y
sudo apt-get install libgmp-dev -y
sudo apt-get install libmpc-dev -y
sudo apt-get install libmpfr-dev -y
sudo apt-get install texinfo -y

#----------------------------------------------------------------
#Actual building

export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.gz
tar xf binutils-2.39.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.39/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
sudo make all install 2>&1 | tee make.log

cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz
tar xf gcc-12.2.0.tar.gz
mkdir gcc-build
cd gcc-build
echo Configure: . . . . . . .
../gcc-12.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-language=c,c++ --without-headers
echo MAKE ALL-GCC:
sudo make all-gcc
echo MAKE ALL-TARGET-LIBGCC:
sudo make all-target-libgcc
echo MAKE INSTALL-GCC:
sudo make install-gcc
echo MAKE INSTALL-TARGET-LIBGCC:
sudo make install-target-libgcc
echo HERE U GO MAYBE:
ls /usr/local/i386elfgcc/bin

export PATH="$PATH:/usr/local/i386elfgcc/bin"
echo 'export PATH="/usr/local/i386elfgcc/bin:$PATH"' >> ~/.bashrc
sudo reboot