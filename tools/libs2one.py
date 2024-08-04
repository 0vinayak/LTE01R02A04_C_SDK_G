#!/usr/bin/env python3
# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.


import os
import sys
import io
import shutil
import getopt
import subprocess


PROJECT_AR = ''
PROJECT_OUT = os.getenv('PROJECT_OUT') + '/'
LIBRARY_IN = ''
LIBRARY_OUT = ''

def usage():
    print("usage:\n\
    --ar   = <ar>       -- cross toolchain ar, need for library\n\
    -i       <input>    -- input library name\n\
    -o       <output>   -- output library name\n\
    --help   <help>     -- show help message\n")

def parseParam(argv):
    global PROJECT_AR, LIBRARY_OUT, LIBRARY_IN

    try:
        opts, args = getopt.getopt(sys.argv[1:],"i:o:",["ar=","help"])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt == '-i':
            LIBRARY_IN = arg
        elif opt == '-o':
            LIBRARY_OUT = arg
        elif opt == '--ar':
            PROJECT_AR = arg
        elif opt == '--help':
            usage()
            sys.exit()

    if PROJECT_AR == '':
        print('failed to get \'PROJECT_AR\' for library')
        sys.exit()
    if LIBRARY_OUT == '':
        print('failed to get \'LIBRARY_OUT\' for library')
        sys.exit()
    if LIBRARY_IN == '':
        print('failed to get \'LIBRARY_IN\' for library')
        sys.exit()

def add_library(library):
    shutil.copyfile(PROJECT_OUT + library, 'temp.a')
    p = subprocess.run([PROJECT_AR, '-t', 'temp.a'],
                         stdout=subprocess.PIPE)
    objs = p.stdout.decode("utf-8").split('\n')

    for obj in objs:
        if obj.find('.obj') != -1:
            # extract file(s) from the archive
            subprocess.run([PROJECT_AR, '-x', 'temp.a', obj])
            # quick append file(s) to the archive
            subprocess.run([PROJECT_AR, '-q', LIBRARY_OUT, obj])
            # delete file(s) from the archive
            subprocess.run([PROJECT_AR, '-d', 'temp.a', obj])
            os.remove(obj)

    os.remove('temp.a')

def main():
    if os.path.exists(LIBRARY_OUT):
        os.remove(LIBRARY_OUT)
    try:
        library_in_h = open(LIBRARY_IN, 'r', encoding='UTF-8')
        libraries = library_in_h.readlines()
        for library in libraries:
            index = library.find('.a')
            if index != -1:
                add_library(library[:index+2])
    except IOError:
        print(sys.exc_info()[0])
        print(sys.exc_info()[1])
    finally:
        library_in_h.close()

    # act as ranlib
    subprocess.run([PROJECT_AR, '-s', LIBRARY_OUT])

    return 0


if __name__ == '__main__':
    parseParam(sys.argv[1:])
    sys.exit(main())