# • 10 - chybějící parametr skriptu (je-li třeba) nebo použití zakázané kombinace parametrů;
# • 11 - chyba při otevírání vstupních souborů (např. neexistence, nedostatečné oprávnění);
# • 12 - chyba při otevření výstupních souborů pro zápis (např. nedostatečné oprávnění, chyba
# při zápisu);
# • 20 – 69 - návratové kódy chyb specifických pro jednotlivé skripty;
# • 99 - interní chyba (neovlivněná vstupními soubory či parametry příkazové řádky; např. chyba
# alokace paměti).
# • 31 - chybný XML formát ve vstupním souboru (soubor není tzv. dobře formátovaný, angl.
# well-formed, viz [1]);
# • 32 - neočekávaná struktura XML (např. element pro argument mimo element pro instrukci,
# instrukce s duplicitním pořadím nebo záporným pořadím) či lexikální nebo syntaktická chyba
# textových elementů a atributů ve vstupním XML souboru (např. chybný lexém pro číselný
# nebo řetězcový literál, neznámý operační kód apod.).
# • 52 - chyba při sémantických kontrolách vstupního kódu v IPPcode21 (např. použití nedefinovaného návěští, redefinice proměnné);
# • 53 - běhová chyba interpretace – špatné typy operandů;
# • 54 - běhová chyba interpretace – přístup k neexistující proměnné (rámec existuje);
# • 55 - běhová chyba interpretace – rámec neexistuje (např. čtení z prázdného zásobníku rámců);
# • 56 - běhová chyba interpretace – chybějící hodnota (v proměnné, na datovém zásobníku nebo
# v zásobníku volání);
# • 57 - běhová chyba interpretace – špatná hodnota operandu (např. dělení nulou, špatná návratová hodnota instrukce EXIT);
# • 58 - běhová chyba interpretace – chybná práce s řetězcem.

import getopt
import sys
import os
import xml.etree.ElementTree as ET

ERROR_OK = 0
ERROR_10 = 10
ERROR_11 = 11
ERROR_31 = 31

# Argument checking
def usage():
    print("Usage: python3.8 inerpret.py [--input=<file>] [--source=<file>]",file=sys.stderr)
    exit(ERROR_10)

if len(sys.argv) == 2 and sys.argv[1] == '--help':
    print("Usage: python3.8 inerpret.py [--input=<file>] [--source=<file>]",file=sys.stderr)
    exit(ERROR_OK)

try:
    opts, args = getopt.getopt(sys.argv[1:], '', ['input=','source='])
except getopt.GetoptError:
    usage()

if len(args) != 0:
    usage()

input_file = ''
source_file = ''

def open_file(file):
    if os.access(file, os.R_OK):
        with open(file) as fp:
            return fp.read()
    else:
        print("ERROR 11: Can not open file",file=sys.stderr)
        exit(ERROR_11)

if len(opts) == 1 and opts[0][0] == '--input' and opts[0][1] != '':
    input_file = open(opts[0][1],"r")
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

try:
    root = ET.fromstring(source_file)
except:
    print("ERROR 31: Not xml file",file=sys.stderr)
    exit(ERROR_31)

