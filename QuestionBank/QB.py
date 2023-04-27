import csv
import random
import socket
from _thread import *

from QBc  import *
from QBpy import *

BUFFERSIZE = 1024

class QuestionBank:
    def __init__(self):
        # Create C and Python QB instances
        self.QBcInstance  = QuestionBankC()
        self.QBpyInstance = QuestionBankPython()
        
        # Initialsie variables
        self.mcqc  = self.QBcInstance.getMCQ()
        self.pcqc  = self.QBcInstance.getPCQ()
        self.mcqpy = self.QBpyInstance.getMCQ()
        self.pcqpy = self.QBpyInstance.getPCQ()

    def getRandom(self):
        # Not including the answers for MCQ
        allQuestions = [x[:-1] for x in self.mcqc] + [x[:-1] for x in self.mcqpy] + self.pcqc + self.pcqpy
        random.shuffle(allQuestions)
        randomQuestions = allQuestions[:10]
        return randomQuestions

    def makeQuestionFile(self, student, password):
        # filename format : student_password
        filename = student + password + ".csv"
        question_list = self.getRandom()
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerows(question_list)
            
    def gradeQuestion(self, fileget):
        file = open(fileget, "r")
        line = file.readline().split(",")
        type = line[0]
        ques = line[1]
        ans  = line[2]
       
        isCorrect = False
        if type == "mcqc":      # C multiple choice question
            isCorrect = self.QBcInstance.gradeMCQ(ques, ans)
        elif type == "pcqc":    # C programming challenge question
            isCorrect = False
        elif type == "mcqpy":   # PYTHON multiple choice question
            isCorrect = self.QBpyInstance.gradeMCQ(ques, ans)
        elif type == "pcqpy":   # PYTHON programming challenge question
            isCorrect = False
        return isCorrect
    
    def runQBserver(self):
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
        print('[+] Binding successful')

        print('[+] Listening...')
        server_socket.listen(5)
        
        # Connects every client to the server simultaneously
        def multi_threaded_client(connection):
            # connection.send(str.encode('Server is working:'))
            while True:
                # Seperately getting data from the clients and returning the server response
                # data = connection.recv(2048) 
                # response = '[+] Server message: ' + data.decode('utf-8')

                data = self.sendQuestionFile("user1", "pass1")
                # response = '[+] Server message: ' + data
                # if not data:
                #     break

                # Send file data
                # connection.sendall(str.encode(data))
                connection.sendall(data)
                break;
            connection.close()
            
        try:
            # Keep connection going
            while True:
                print('[+] Waiting...')
                client, address = server_socket.accept()
                # Connected client's information such as thread number and adddress given to it
                print('[+] Connected to: ' + address[0] + ':' + str(address[1]))
                start_new_thread(multi_threaded_client, (client, ))
                thread_count += 1
                print('[+] Thread Number: ' + str(thread_count))
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            server_socket.close()    # close QB socket
            print("[-] Connection closed.")
        

    
    
    '''# Sending data via sockets
    def sendTM(self, QBsocket, filesent):
        HOST = "127.0.0.1"  # host
        PORT = 8080         # port
        
        # open and read file to be transferred
        with open(filesent, "rb") as f:
            QBdata = f.read()
        
        # send file data
        QBsocket.sendall(QBdata)
        print("[+] File sent successfully.")
        QBsocket.shutdown(socket.SHUT_RD)
        
    # Receiving data via sockets
    def receiveTM(self, QBsocket, fileget):
        HOST = "127.0.0.1"  # host
        PORT = 8080         # port

        # receive file data
        TMdata = QBsocket.recv(BUFFERSIZE)
        
        # create and write file received
        with open(fileget, 'wb') as f:
            f.write(TMdata)
        print("[+] File received successfully.")
        
        # send response after grading
        isCorrect = self.gradeQuestion(fileget)
        if (isCorrect):
            QBsocket.send("CORRECT".encode())
            print("[+] Response 'CORRECT' sent successfully.")
        else:
            QBsocket.send("WRONG".encode())
            print("[+] Response 'WRONG' sent successfully.")
        

    def QBsocket(self, student, password):
        HOST = "127.0.0.1"  # host
        PORT = 8080         # port
        FILESENT = student + password + ".csv"
        FILEGET  = student + password + "ANS.csv"
        
        # Create the client socket
        QBsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sending = True
        
        # connecting to TM to send
        print(f"[+] Connecting to {HOST}:{PORT}.")
        QBsocket.connect((HOST, PORT))
        print("[+] Connected.")

        try:
            # Keep connection going
            while True:
                # First QB sends data (questions), then it receives data (answers to grade)
                if sending:
                    self.makeQuestionFile(student, password)
                    self.sendTM(QBsocket, FILESENT)
                    sending = False
                else:
                    self.receiveTM(QBsocket, FILEGET)
                    sending = True
                
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            QBsocket.close()    # close QB socket
            print("[-] Connection closed.")'''
        
            
    
QB = QuestionBank()
QB.runQBserver()

# # binding to TM to receive
# print(f"[+] Binding to {HOST}:{PORT}.")
# # QBsocket.bind((HOST, PORT))
# print("[+] Binded.")

# # listening for data
# QBsocket.listen(10)
# print(f"[*] Listening as {HOST}:{PORT}")
# TMsocket, TMaddress = QBsocket.accept() 
# print(f"[+] {TMaddress} is connected.")