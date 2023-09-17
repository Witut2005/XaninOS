
import os
import sys
import subprocess
    
def main():
    crtbegin_path = subprocess.check_output('i386-elf-gcc $CFLAGS -print-file-name=crtbegin.o', shell=True, text=True)[:-1]
    crtend_path = subprocess.check_output('i386-elf-gcc $CFLAGS -print-file-name=crtend.o', shell=True, text=True)[:-1]

    os.system('sudo cp ' + crtbegin_path + ' ../files/') 
    os.system('sudo cp ' + crtend_path + ' ../files/')

if __name__ == '__main__':
    main()
