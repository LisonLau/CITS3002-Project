import socket
import os
from _thread import *
# src: https://www.positronx.io/create-socket-server-with-multiple-clients-in-python/
# src2: https://realpython.com/python-sockets/#background 

# echo-server.py

import socket

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)

# create socket connection
# AF_INET is the Internet address family for IPv4. SOCK_STREAM is the socket type for TCP
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # connect the host and port to the socket server
    try:
        s.bind((HOST, PORT))
    except socket.error as e:
        print(str(e))

    # awaits for a connection request from client
    s.listen(5)

    client_conn, addr = s.accept()
    with client_conn:
        print(f"Connected by {addr}")
        while True:
            data = client_conn.recv(1024)
            if not data:
                break
            client_conn.sendall(data)