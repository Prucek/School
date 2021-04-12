# Author: Peter Rucek, xrucek00
# File: interpret.py
# Date: 12.4.2021
# Description: Parses arguments and reads XML, main program

import getopt
import sys
import os
import xml.etree.ElementTree as ET
import exec
import library as l


# Argument checking
def usage():
    error(10,"Usage: python3.8 inerpret.py [--input=<file>] [--source=<file>]")

if len(sys.argv) == 2 and sys.argv[1] == '--help':
    error(0,"Usage: python3.8 inerpret.py [--input=<file>] [--source=<file>]")

try:
    opts, args = getopt.getopt(sys.argv[1:], '', ['input=','source='])
except getopt.GetoptError:
    usage()

if len(args) != 0:
    usage()

input_file = ''
source_file = ''

def open_file(file):
    if os.access(file, os.R_OK) and os.path.isfile(file):
        with open(file) as fp:
            return fp.read()
    else:
        error(11,"ERROR 11: Can not open file")

# Read files
if len(opts) == 1 and opts[0][0] == '--input' and opts[0][1] != '':
    input_file = open_file(opts[0][1])
    source_file = sys.stdin

elif len(opts) == 1 and opts[0][0] == '--source' and opts[0][1] != '':
    input_file = sys.stdin
    source_file = open_file(opts[0][1])

elif len(opts) == 2 and opts[0][1] != '' and opts[1][1] != '':
    if opts[0][0] == '--source':
        input_file = open_file(opts[1][1])
        source_file = open_file(opts[0][1])
    else:
        input_file = open_file(opts[0][1])
        source_file = open_file(opts[1][1])
else:
    usage()


if input_file == sys.stdin:
    input_file = ''
    for line in sys.stdin:
        input_file += line

if source_file == sys.stdin:
    source_file = ''
    for line in sys.stdin:
        source_file += line

# Read XML
try:
    root = ET.fromstringlist(source_file)
except:
    error(31,"ERROR 31: Not xml file")

# Root check
if root.tag != 'program':
    error(32,"ERROR 32: Wrong xml file")


list_of_root_atrib = list(root.attrib.items())
if len(list_of_root_atrib) > 3:
    error(32,"ERROR 32: Wrong xml file")

is_ok = False
for i in range(len(list_of_root_atrib)):
    if list_of_root_atrib[i][0] == 'language' and list_of_root_atrib[i][1] == 'IPPcode21':
        is_ok = True
    elif list_of_root_atrib[i][0] != 'name' and list_of_root_atrib[i][0] != 'description':
        error(32,"ERROR 32: Wrong xml file")

if not is_ok:
    error(32,"ERROR 32: Wrong xml file")

interpret = exec.Interpret()


# Child check 
for child in root:
    if child.tag != 'instruction':
        error(32,"ERROR 32: Wrong xml file")
    if 'opcode' not in child.attrib or 'order' not in child.attrib: # order and opcode are required
        error(32,"ERROR 32: Wrong xml file")

    opcode = ''
    order = 0

    for attrib, value in child.attrib.items():
        if attrib == 'opcode':
            opcode = value
        if attrib == 'order':
            try:
                order = int(value)
            except:
                error(32,"ERROR 32: Wrong xml file")
            if order <= 0:
                error(32,"ERROR 32: Wrong xml file")

    args = []
    for arg in child:

        if arg.tag != 'arg1' and arg.tag != 'arg2' and arg.tag != 'arg3':
            error(32,"ERROR 32: Wrong xml file")

        if 'type' not in arg.attrib:
            error(32,"ERROR 32: Wrong xml file")

        
        if arg.attrib['type'] not in l.TYPES:
            error(32,"ERROR 32: Wrong xml file")

        args.append((arg.attrib['type'],arg.text))
        for error in arg:
            if error != '':
                error(32,"ERROR 32: Wrong xml file")

    interpret.add_instruction(opcode.upper(),order,args)

interpret.execute(input_file)