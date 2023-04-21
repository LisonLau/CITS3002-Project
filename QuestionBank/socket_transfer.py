import socket

from questions_maker import *

def sendQuestionFile(student, password):
    BUFFERSIZE = 1024
    FORMAT = "utf-8"
    host = "127.0.0.1"  # host
    port = 8080         # port
    filename = makeQuestionFile(student, password)
    
    # create the client socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print(f"[+] Connecting to {host}:{port}")
    s.connect((host, port))
    print("[+] Connected.")
    
    # open and read file to be transferred
    filename = makeQuestionFile(student, password)
    with open(filename, "rb") as f:
        data = f.read()

    # send file data
    s.sendall(data)
    print("[+] File sent successfully.")
    
    s.close()    # close the socket