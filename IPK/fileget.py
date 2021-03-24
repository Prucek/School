#!/usr/bin/python3.7
"""
    Author: Peter Rucek, xrucek00
    Date: 24.3.2021
    Project: IPK project 1 - client for File Transfer Protocol
"""

import socket
import sys
import getopt
import re
import time
import os
import select

# Macros

FORMAT = "utf-8"
USAGE_ERR = 2
SOCKET_ERR = 3
ERROR = -1

# Functions

def usage():
    print("Usage : ./fileget -n <ip_adress>:<port> -f fsp://SERVER_NAME/PATH/TO/file.txt")
    print("Wrong arguments")
    sys.exit(USAGE_ERR)


def parse_adress_and_port(adr):
    a = adr.split(":")
    if len(a) != 2:
        usage()
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


def parse_surl(surl):
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
        disconnect("ERROR: Could not connect!", s)
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
        return ERROR, ERROR
    else:
        if len(msg) == 0:
            return ERROR, ERROR
    while True:
        try:
            decoded = msg.decode(FORMAT,'ignore')
        except:
            continue
        break
    if (decoded == ""):
        return ERROR, ERROR

    return decoded, msg


def parse_msg(msg, sock):
    a = msg.split(" ")
    if a[0] == "OK":
        return a[1]
    if a[0] == "ERR":
        disconnect("ERROR: Could not find file or server!", sock)


def get_msg_len(msg_dec, msg_not_dec, sock):

    a = msg_dec.split("\r\n\r\n")
    header = a[0].split(":")
    length = header[1].strip()
    header_len = len(a[0]) + len("\r\n\r\n")

    val = msg_dec.find("Success")
    if val == ERROR:
        disconnect("ERROR: Could not connect!", sock)
    else:
        byte_msg = bytearray(msg_not_dec[header_len:])
        return int(length), byte_msg

def get_byte_data(msg_dec, msg_not_dec, sock):
    length, msg_from_header = get_msg_len(msg_dec, msg_not_dec, sock)
    byte_msg = bytearray(msg_from_header)

    sock.setblocking(0)
    begin = time.time()
    timeout = 20
    new = []
    old = []

    while len(byte_msg) < length:
        if time.time()-begin > timeout:
            return ERROR
        try:
            new = sock.recv(length - len(byte_msg))
        except:
            pass

        if new and old != new:
            byte_msg += new
            old = new
            begin=time.time()
        else:
            time.sleep(0.1)

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

adress, port = parse_adress_and_port(name_server)
check_protocol(surl)

hostname, path_and_file = parse_surl(surl)

nsp_socket = connect(adress, port, socket.SOCK_DGRAM) # UDP

send("WHEREIS "+ hostname + "\r\n", nsp_socket) # NSP protocol
msg , _= decoded_recv(nsp_socket)
if msg == ERROR:
    disconnect("ERROR: [NSP] Could not receive or decode data!",nsp_socket)

add_and_port = parse_msg(msg, nsp_socket)
adress, port = parse_adress_and_port(add_and_port)
nsp_socket.close()

fsp_socket = connect(adress, port, socket.SOCK_STREAM) #TCP

a = path_and_file.split("/")
file = a[len(a)-1]

print("Downloding",file,"...")

# FSP protocol 
if (file == "*"): # GET ALL
    send("GET index FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)
else: # GET single
    send("GET " + path_and_file + " FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)


decoded, not_decoded = decoded_recv(fsp_socket)
if decoded == ERROR:
    disconnect("ERROR: [FSP] Could not receive or decode data!",fsp_socket)

byte_msg = get_byte_data(decoded, not_decoded, fsp_socket)
if byte_msg == ERROR:
    disconnect("ERROR: Could not receive data!",fsp_socket)

f = open(file, "wb")
f.write(byte_msg)
f.close()
fsp_socket.close()

print("         read successfully!")

if(file == "*"): # GET ALL
    index = open("*","r")
    lines = index.readlines()
    lines = map(lambda s: s.strip(), lines)
    for i in lines:
        fsp_socket = connect(adress, port, socket.SOCK_STREAM)
        send("GET " + i + " FSP/1.0\r\nHostname: " + hostname + "\r\nAgent: xrucek00\r\n\r\n",fsp_socket)

        print("Downloding",i,"...")

        decoded, not_decoded = decoded_recv(fsp_socket)
        if decoded == ERROR:
            fsp_socket.close()
            print("ERROR: File recv",i," was not read!")
            continue

        byte_msg = get_byte_data(decoded, not_decoded, fsp_socket)
        if byte_msg == ERROR:
            fsp_socket.close()
            print("ERROR: File byte",i," was not read!")
            continue

        a = i.split("/")
        i = a[len(a)-1]
        
        f = open(i, "wb")
        f.write(byte_msg)
        f.close()

        print("         read successfully!")
        fsp_socket.close()

    index.close()
    os.remove("*")



######################################