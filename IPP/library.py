# Author: Peter Rucek, xrucek00
# File: library.py
# Date: 12.4.2021
# Description: Some constants and error priniting

import sys

INTCONS = '(^int@(\+|\-|)\d+$)'
BOLCONS = '(^bool@(true|false)$)'
STRCONS = '(^string@(([^\s#\\\\]|\\\\\d{3})*)$)'
NILCONS = '(^nil@nil$)'

VARIABLE = '(^(GF|TF|LF)@([a-z]|[A-Z]|_|\-|\$|\&|\%|\*|\!|\?)([a-z]|[A-Z]|[0-9]|_|\-|\$|\&|\%|\*|\!|\?)*$)'
CONSTANT = '('+INTCONS+'|'+BOLCONS+'|'+STRCONS+'|'+NILCONS+')'
TYPE = '^(int|bool|string)$'
LABEL = '^([a-z]|[A-Z]|_|\-|\$|\&|\%|\*|\!|\?)([a-z]|[A-Z]|[0-9]|_|\-|\$|\&|\%|\*|\!|\?)*$'
SYMBOL = '('+CONSTANT+'|'+VARIABLE+')'


INSTRUCTIONS = {

    'CREATEFRAME' : [],
    'PUSHFRAME'   : [],
    'POPFRAME'    : [],
    'RETURN'      : [],
    'BREAK'       : [],

    'EXIT' : [SYMBOL],
    'DPRINT' : [SYMBOL],
    'WRITE' : [SYMBOL],
    'PUSHS' : [SYMBOL],
    'JUMP' : [LABEL],
    'LABEL' : [LABEL],
    'CALL' : [LABEL],
    'POPS' : [VARIABLE],
    'DEFVAR' : [VARIABLE],

    'MOVE'     : [VARIABLE,SYMBOL],
    'INT2CHAR' : [VARIABLE,SYMBOL],
    'READ'     : [VARIABLE,TYPE],
    'STRLEN'   : [VARIABLE,SYMBOL],
    'TYPE'     : [VARIABLE,SYMBOL],
    'NOT' : [VARIABLE,SYMBOL],
    'ADD' : [VARIABLE,SYMBOL,SYMBOL],
    'SUB' : [VARIABLE,SYMBOL,SYMBOL],
    'MUL' : [VARIABLE,SYMBOL,SYMBOL],
    'IDIV': [VARIABLE,SYMBOL,SYMBOL],
    'LT'  : [VARIABLE,SYMBOL,SYMBOL],
    'GT'  : [VARIABLE,SYMBOL,SYMBOL],
    'EQ'  : [VARIABLE,SYMBOL,SYMBOL],
    'AND' : [VARIABLE,SYMBOL,SYMBOL],
    'OR'  : [VARIABLE,SYMBOL,SYMBOL],

    'STRI2INT' : [VARIABLE,SYMBOL,SYMBOL],
    'CONCAT'   : [VARIABLE,SYMBOL,SYMBOL],
    'GETCHAR'  : [VARIABLE,SYMBOL,SYMBOL],
    'SETCHAR'  : [VARIABLE,SYMBOL,SYMBOL],
    'JUMPIFEQ' : [LABEL,SYMBOL,SYMBOL],
    'JUMPIFNEQ': [LABEL,SYMBOL,SYMBOL]
}

TYPES = ['int', 'bool', 'string', 'label', 'type', 'var', 'label', 'nil']


def error(err_code, msg):
    print(msg,file=sys.stderr)
    exit(err_code)