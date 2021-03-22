#!/usr/bin/python3.7

import socket
import sys
import getopt
import re
import time
import os

# Macros

FORMAT = "utf-8"
USAGE_ERR = 2
SOCKET_ERR = 3

# Functions

def usage():
    print("Usage : ./fileget -n <ip_adress>:<port> -f fsp://SERVER_NAME/PATH/TO/file.txt")
    print("Wrong arguments")
    sys.exit(USAGE_ERR)


def get_adress(adr):
    a = adr.split(":")
    try:
        socket.inet_aton(a[0])
    except socket.error:
        usage()
    if not a[1].isnumeric():
        usage()
    return a[0], a[1]


def check_protocol(p):
    match = re.search("^fsp://(\w|\.|\-)*/.", p)
    if match == None:
        usage()


def get_hostname(surl):
    s = surl.replace("fsp://","")
    hostname = s.split("/", 1)[0]
    path_and_file = s.split("/", 1)[1]
    return hostname, path_and_file


def connect(adress, port, sock_type):
    s = socket.socket(socket.AF_INET, sock_type)
    s.settimeout(5)
    try:
        s.connect((adress, int(port)))
    except:
        disconnect("Could not connect!", s)
    return s


def disconnect(err_msg, sock):
    print(err_msg)
    sock.close()
    exit(SOCKET_ERR)


def send(msg, sock):
    message = msg.encode(FORMAT)
    sock.send(message)


def decoded_recv(sock):
    try:
        msg = sock.recv(64) # should be enough for header
    except:
        disconnect("Could not connect!", sock)
    
    while True:
        try:
            decoded = msg.decode(FORMAT)
        except:
            continue
        break

    if (decoded == ""):
        disconnect("Did no recieve anything!", fsp_socket)

    return decoded


def parse_msg(msg, sock):
    a = msg.split(" ")
    if a[0] == "OK":
        return a[1]
    if a[0] == "ERR":
        disconnect("Could not connect!\n" + msg, sock)


def get_msg_len(msg, sock):
    a = msg.split("\r\n\r\n")
    header = a[0].split(":")
    length = int(header[1].strip())
    header_len = len(a[0]) + 4 # \r\n\r\n

    val = msg.find("Success")
    if val == -1:
        new = sock.recv(length)
        msg += new.decode(FORMAT)
        disconnect(msg, sock)
    else:
        byte_msg = bytearray(msg[header_len:], encoding = FORMAT)
        return length, byte_msg

def get_byte_data(msg, sock):
    length, msg_from_header = get_msg_len(msg, fsp_socket)
    byte_msg = bytearray(msg_from_header)
    while len(byte_msg) < length:
        new = fsp_socket.recv(length)
        byte_msg += new
    return byte_msg

### Main ###
######################################
# Argument checking

try:
    opts, args = getopt.getopt(sys.argv[1:], "n:f:")
except getopt.GetoptError:
    usage()

if (len(opts) != 2) or args:
    usage()

if (opts[0][0] == opts[1][0]):
    usage()

for i in opts:
    if(i[0] == "-n"):
        name_server = i[1]
    if(i[0] == "-f"):
        surl = i[1]

# Socketing

adress, port = get_adress(name_server)
check_protocol(surl)

hostname, path_and_file = get_hostname(surl)

nsp_socket = connect(adress, port, socket.SOCK_DGRAM)

send("WHEREIS "+ hostname + "\r\n", nsp_socket)
msg = decoded_recv(nsp_socket)

add_and_port = parse_msg(msg, nsp_socket)
adress, port = get_adress(add_and_port)
nsp_socket.close()

fsp_socket = connect(adress, port, socket.SOCK_STREAM)

a = path_and_file.split("/")
file = a[len(a)-1]
if (file == "*"):
    send("GET index FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)
else:
    send("GET " + path_and_file + " FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)


msg = decoded_recv(fsp_socket)

byte_msg = get_byte_data(msg, fsp_socket)

f = open(file, "wb")
f.write(byte_msg)
f.close()
fsp_socket.close()

if(file == "*"):
    index = open("*","r")
    lines = index.readlines()
    lines = map(lambda s: s.strip(), lines)
    for i in lines:
        fsp_socket = connect(adress, port, socket.SOCK_STREAM)
        send("GET " + i + " FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)
        msg = decoded_recv(fsp_socket)
        byte_msg = get_byte_data(msg, fsp_socket)
        
        a = i.split("/")
        i = a[len(a)-1]
        
        f = open(i, "wb")
        f.write(byte_msg)
        f.close()

    index.close()
    os.remove("*")



######################################