#!/usr/bin/python3.7

import socket
import sys
import getopt
import re

FORMAT = "utf-8"
ERR_CODE = 2

def usage():
    print("Usage : ./fileget -n <ip_adress>:<port> -f fsp://SERVER_NAME/PATH/TO/file.txt")
    print("Wrong arguments")
    sys.exit(ERR_CODE)

# Argument checking
try:
    opts, args = getopt.getopt(sys.argv[1:], "n:f:")
except getopt.GetoptError as err:
    print(err)
    sys.exit(ERR_CODE)

if (len(opts) != 2) or args:
    usage()

if (opts[0][0] == opts[1][0]):
    usage()

for i in opts:
    if(i[0] == "-n"):
        name_server = i[1]
    if(i[0] == "-f"):
        surl = i[1]

def check_adress(adr):
    a = adr.split(":")
    try:
        socket.inet_aton(a[0])
    except socket.error:
        usage()
    if not a[1].isnumeric():
        usage()
    return a[0], a[1]

def check_protocol(p):
    match = re.search("^fsp://(\w|\.|\-)*/.",p)
    if match == None:
        usage()

adress, port = check_adress(name_server)
check_protocol(surl)



# s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
# s.connect(('127.0.0.2',1234))

# answer = ''

# def send(msg):
#     message = msg.encode(FORMAT)
#     s.send(message)
#     # answer = s.recv(2048).decode(FORMAT)

# send("WHEREIS file.server.one\r\n")
# #send("GET index FSP/1.0\r\nHostname: file-server-two\r\nAgent: xrucek00\r\n\r\n'")
# # input()
# msg = s.recv(1024)
# print(msg.decode("utf-8"))
# input()