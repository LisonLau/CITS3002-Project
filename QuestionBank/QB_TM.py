# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import csv
import random
import socket
import time
from _thread import *
from QBc  import *
from QBpy import *

class QuestionBank:
    def __init__(self, server_host):
        self.SERVER_HOST = server_host
        self.SERVER_PORT = 8888
        self.BUFFERSIZE  = 1024
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

    def makeQuestionFile(self, filename):
        # filename format : student_password
        question_list = self.getRandom()
        try:
            with open(filename, 'w', newline='') as csvfile:
                writer = csv.writer(csvfile)
                writer.writerows(question_list)
        except Exception as e:
            print(f"Error occurred: {str(e)}")
            
    def gradeQuestion(self, type, ques, ans):
        isCorrect = False
        if type == "mcqc":      # C multiple choice question
            isCorrect = self.QBcInstance.gradeMCQ(ques, ans)
        elif type == "pcqc":    # C programming challenge question
            isCorrect = self.QBcInstance.gradePCQ(ques, ans)
        elif type == "mcqpy":   # PYTHON multiple choice question
            isCorrect = self.QBpyInstance.gradeMCQ(ques, ans)
        elif type == "pcqpy":   # PYTHON programming challenge question
            isCorrect = self.QBpyInstance.gradePCQ(ques, ans)
        else:
            print("Error occurred: invalid question type")
        return isCorrect

    def getAnswer(self, type, ques):
        answer = ""
        if type == "mcqc":      # C multiple choice question
            answer = self.QBcInstance.getMCQanswer(ques)
        elif type == "pcqc":    # C programming challenge question
            answer = self.QBcInstance.getPCQanswer(ques)
        elif type == "mcqpy":   # PYTHON multiple choice question
            answer = self.QBpyInstance.getMCQanswer(ques)
        elif type == "pcqpy":   # PYTHON programming challenge question
            answer = self.QBcInstance.getPCQanswer(ques)
        else:
            print("Error occurred: invalid question type")
        return answer
    
    def categoriseMessage(self, message):
        if len(message.split("@")) == 2:
            return "get_file"
        elif len(message.split("@")) == 3:
            return "check"
        elif len(message.split("@")) == 4:
            return "get_ans"
        else:
            print("Error occurred: invalid message")
            return ""
        
    def executeSendFile(self, message, TMsocket):
        # Get filename and create question file
        filename = message.split("@")[1]
        self.makeQuestionFile(filename)
        
        # Check if the file exists
        if os.path.isfile(filename):
            # Open the file, read its contents and send it to the client
            file = open(filename, 'r')
            file_contents = file.read()
            TMsocket.send(file_contents.encode())
            print(f"[+] Question file '{filename}' sent successfully.")
        else:
            print("[-] Failed to send question file.")   
    
    def executeCheckAnswer(self, message, TMsocket):
        # Grade question
        type, question, answer = message.split("@")
        isCorrect = self.gradeQuestion(type, question, answer)
        # Send response 'correct' or 'wrong'
        try:
            if (isCorrect):
                TMsocket.send("correct".encode())
                print("[+] Response 'correct' sent successfully.")
            else:
                TMsocket.send("wrong".encode())
                print("[+] Response 'wrong' sent successfully.")
        except Exception as e:
            print(f"Error occurred: {str(e)}")
            
    def executeSendAnswer(self, message, TMsocket):
        # Find correct answer for given question
        try:
            type = message.split("@")[2]
            question = message.split("@")[3]
            correctAns = self.getAnswer(type, question)
            TMsocket.send(correctAns.encode())
            print(f"[+] Answer '{correctAns}' sent successfully.")
        except Exception as e:
            print(f"Error occurred: {str(e)}")
        
    def sendToTM(self, message, TMclient):
        # Categorise message received
        if self.categoriseMessage(message) == "get_file":
            self.executeSendFile(message, TMclient)
        elif self.categoriseMessage(message) == "check":
            self.executeCheckAnswer(message, TMclient)
        elif self.categoriseMessage(message) == "get_ans":
            self.executeSendAnswer(message, TMclient)
    
    def printReceivedMsg(self, message):
        # Categorise message received
        if self.categoriseMessage(message) == "get_file":
            print("[+] Message 'get file' from TM received.")
        elif self.categoriseMessage(message) == "check":
            print("[+] Message 'check answer' from TM received.")
        elif self.categoriseMessage(message) == "get_ans":
            print("[+] Message 'get answer' from TM received.")
        else:
            print("[!] Error: message received was not understood.")
            
    def receiveACK(self, message, TMclient):
        ack = ""
        while (ack != "ACK"):
            ack = TMclient.recv(self.BUFFERSIZE).decode()
            if ack == "ACK":    # ack received, can close connection
                print("[+] Acknowledgement from TM received.")
                break
            else:               # ack not received, retry sending data
                print("[-] Acknowledgement from TM not received.")
                print("[.] Retrying in 5 seconds...")
                time.sleep(5)
                self.sendToTM(message, TMclient)
    
    def runQBserver(self):
        try:
            # Create server socket
            QBserver = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print('[+] Server socket created.')
            
            # Set socket opt
            QBserver.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            print('[+] Set socket options successful.')
            
            # Bind socket to host and port
            QBserver.bind((self.SERVER_HOST, self.SERVER_PORT))
            print('[+] Binding successful.')

            # Listen for connections
            QBserver.listen(5)
            print(f"[*] Listening as {self.SERVER_HOST}:{self.SERVER_PORT}")
        except socket.error as e:
            print(f'[-] Error in creating QB socket: {str(e)}')
            QBserver.close()
        
        try:
            while True:
                # Establish connection 
                TMclient, TMaddress = QBserver.accept()
                print("----- Connected to: " + TMaddress[0] + ':' + str(TMaddress[1]) + " ------------")
                
                # Receive a string message
                message = TMclient.recv(self.BUFFERSIZE).decode()
                
                # Send acknowledgment for received data
                if message:
                    self.printReceivedMsg(message)
                    TMclient.send("ACK".encode())
                    print("[+] Message 'ACKNOWLEDGEMENT' sent successfully.")
                else:
                    break
                    
                # Perform the required send operation
                time.sleep(0.1) # to prevent simultaneous ACK and message send
                self.sendToTM(message, TMclient)

                # Receive TM acknowledgement for sent data
                self.receiveACK(message, TMclient)
                    
                TMclient.close()
                print("----- Connection to " + TMaddress[0] + ':' + str(TMaddress[1]) + " closed -----")
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            QBserver.close()    # close QB socket
            print("[-] QB server connection closed.")