import socket
from _thread import *
from socket_transfer import *

def runQbServer():
    host = '127.0.0.1'
    port = 8888
    thread_count = 0
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print('[+] Server socket created.')

    # # Connect is usually used by the client to reach to the server
    # server_socket.connect((host, port))
    # print("[+] Connected.")

    try:
        server_socket.bind((host, port))
    except socket.error as e:
        print(f'[-] {str(e)}')
        return 0
    print('[+] Binding successfull')

    print('[+] Listening...')
    server_socket.listen(5)
    
    # Connects every client to the server simultaneously
    def multi_threaded_client(connection):
        # connection.send(str.encode('Server is working:'))
        while True:
            # Seperately getting data from the clients and returning the server response
            # data = connection.recv(2048) 
            # response = '[+] Server message: ' + data.decode('utf-8')

            data = sendQuestionFile("user1", "pass1")
            # response = '[+] Server message: ' + data
            # if not data:
            #     break

            # Send file data
            # connection.sendall(str.encode(data))
            connection.sendall(data)
            break;
        connection.close()
    
    while True:
        print('[+] Waiting...')
        client, address = server_socket.accept()
        # Connected client's information such as thread number and adddress given to it
        print('[+] Connected to: ' + address[0] + ':' + str(address[1]))
        start_new_thread(multi_threaded_client, (client, ))
        thread_count += 1
        print('[+] Thread Number: ' + str(thread_count))
    server_socket.close()