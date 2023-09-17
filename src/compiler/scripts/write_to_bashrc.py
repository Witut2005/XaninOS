
import os
import sys

def main(argv):

    if len(argv) != 3:
        print('ERROR invalid number of arguments')
        sys.exit(1)

    elif argv[1].split('/')[-1] != 'xaninOS' or argv[2].split('/')[-1] != 'bin':
        print('ERROR: invalid arguments')
        sys.exit(1)

    print('XANIN HOME: ', argv[1])
    print('CROSS COMPILER HOME: ', argv[2])

    os.system('echo export XANIN_HOME=' + argv[1] + ' >> ~/.bashrc')
    os.system('echo \'export PATH=' + argv[2] + ':$PATH\' >> ~/.bashrc')

if __name__ == '__main__':
    main(sys.argv)