import socket
import os
from _thread import *
# src: https://www.positronx.io/create-socket-server-with-multiple-clients-in-python/
# src2: https://realpython.com/python-sockets/#background 

# create socket connection
# AF_INET is the Internet address family for IPv4. SOCK_STREAM is the socket type for TCP
ServerSideSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '127.0.0.1'  # Standard loopback interface address (localhost)
port = 65432        # Port to listen on (non-privileged ports are > 1023)
ThreadCount = 0

# connect the host and port to the socket server
try:
    ServerSideSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Socket is listening..')
ServerSideSocket.listen(5)

# connects every client from the various address provided by the server simultaneously
def multi_threaded_client(connection):
    connection.send(str.encode('Server is working:'))
    while True:
        # seperately getting the data from every client and returning the response to a specific client
        data = connection.recv(2048) 
        response = 'Server message: ' + data.decode('utf-8')
        if not data:
            break
        connection.sendall(str.encode(response))
    connection.close()

# server connection runs constant
while 1:
    Client, address = ServerSideSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    # handles the connection, establishes a new thread for every client
    start_new_thread(multi_threaded_client, (Client, ))
    ThreadCount += 1
    print('Thread Number: ' + str(ThreadCount))

# ServerSideSocket.close()