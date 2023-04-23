import csv
import random
import socket

from QBc  import *
from QBpy import *

BUFFERSIZE = 1024

class QuestionBank:
    def __init__(self):
        # Create C and Python QB instances
        QBcInstance  = QuestionBankC()
        QBpyInstance = QuestionBankPython()
        
        # Initialsie variables
        self.mcqc  = QBcInstance.getMCQ()
        self.pcqc  = QBcInstance.getPCQ()
        self.mcqpy = QBpyInstance.getMCQ()
        self.pcqpy = QBpyInstance.getPCQ()

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
    
    def QBsocket(self, student, password):
        HOST = "127.0.0.1"  # host
        PORT = 8080         # port
        FILESENT = student + password + ".csv"
        FILEGET  = student + password + "ANS.csv"
        
        try:
            # Create the client socket
            QBsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # Connecting and Binding to TM
            print(f"[+] Connecting to {HOST}:{PORT}.")
            QBsocket.connect((HOST, PORT))
            print("[+] Connected.")
            print(f"[+] Binding to {HOST}:{PORT}.")
            QBsocket.bind((HOST, PORT))
            print("[+] Binded.")
            
            try:
                # Keep connection going
                while True:
                    # SENDING DATA
                    # open and read file to be transferred
                    with open(FILESENT, "rb") as f:
                        QBdata = f.read()
                    
                    # send file data
                    QBsocket.sendall(QBdata)
                    print("[+] File sent successfully.")
                    
                    # RECEIVING DATA
                    QBsocket.listen(10)
                    print(f"[*] Listening as {HOST}:{PORT}")
                    TMsocket, TMaddress = QBsocket.accept() 
                    print(f"[+] {TMaddress} is connected.")
                    
                    # receive file data
                    TMdata = TMsocket.recv(BUFFERSIZE)
                    
                    # create and write file received
                    with open(FILEGET, 'wb') as f:
                        f.write(TMdata)
                    print("[+] File received successfully.")
            
            # If the user presses Ctrl+C, close the connection and the socket
            except KeyboardInterrupt:
                TMsocket.close()    # close TM socket
                QBsocket.close()    # close QB socket
                print("[-] Connection closed.")
        
        except Exception:
            print("[-] Error has occurred.")
            
    
QB = QuestionBank()
QB.QBsocket("user1", "pass1")