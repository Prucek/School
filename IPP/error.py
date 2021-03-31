import sys

ERROR_OK = 0
ERROR_10 = 10
ERROR_11 = 11
ERROR_31 = 31
ERROR_32 = 32

def error(err_code, msg):
    print(msg,file=sys.stderr)
    exit(err_code)