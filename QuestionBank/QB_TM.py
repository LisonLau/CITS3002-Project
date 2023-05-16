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
        # Network socket variables
        self.SERVER_HOST = server_host
        self.SERVER_PORT = 8888
        
        # Create C and Python QB instances
        self.QBcInstance  = QuestionBankC()
        self.QBpyInstance = QuestionBankPython()
        
        # Initialsie variables
        self.mcqc  = self.QBcInstance.getMCQ()
        self.pcqc  = self.QBcInstance.getPCQ()
        self.mcqpy = self.QBpyInstance.getMCQ()
        self.pcqpy = self.QBpyInstance.getPCQ()
        
        self.BUFFERSIZE  = 1024
        self.filesList = []

    # Generates a list of random questions from MCQ and PCQ 
    def getRandom(self):
        # Not including the answers for MCQ
        allQuestions = [x[:-1] for x in self.mcqc] + [x[:-1] for x in self.mcqpy] + self.pcqc + self.pcqpy
        random.shuffle(allQuestions)
        randomQuestions = allQuestions[:10]
        return randomQuestions

    # Create a file containing the questions for a student
    def makeQuestionFile(self, filename):
        # filename format : student_password
        question_list = self.getRandom()
        try:
            with open(filename, 'w', newline='') as csvfile:
                writer = csv.writer(csvfile)
                writer.writerows(question_list)
            self.filesList.append(filename)
        except Exception as e:
            print(f"Error occurred: {str(e)}")
            
    # Grade the question and return whether it is correct or not
    def gradeQuestion(self, type, ques, ans):
        isCorrect = False
        output = " "
        if type == "mcqc":      # C multiple choice question
            isCorrect = self.QBcInstance.gradeMCQ(ques, ans)
        elif type == "pcqc":    # C programming challenge question
            isCorrect, output = self.QBcInstance.gradePCQ(ques, ans)
        elif type == "mcqpy":   # PYTHON multiple choice question
            isCorrect = self.QBpyInstance.gradeMCQ(ques, ans)
        elif type == "pcqpy":   # PYTHON programming challenge question
            isCorrect, output = self.QBpyInstance.gradePCQ(ques, ans)
        else:
            print("Error occurred: invalid question type")
        return isCorrect, output

    # Retrieve the answer to the given question
    def getAnswer(self, type, ques, getImg):
        answer = ""
        if type == "mcqc":      # C multiple choice question
            answer = self.QBcInstance.getMCQanswer(ques)
        elif type == "pcqc":    # C programming challenge question
            if getImg == 1:
                answer = self.QBcInstance.getPCQimage(ques) # in image form
            else:
                answer = self.QBcInstance.getPCQanswer(ques)
        elif type == "mcqpy":   # PYTHON multiple choice question
            answer = self.QBpyInstance.getMCQanswer(ques)
        elif type == "pcqpy":   # PYTHON programming challenge question
            if getImg == 1:
                answer = self.QBpyInstance.getPCQimage(ques) # in image form
            else:
                answer = self.QBpyInstance.getPCQanswer(ques)
        else:
            print("Error occurred: invalid question type")
        return answer
        
    # Send the question file to TM
    def executeSendFile(self, message, TMsocket):
        # Get filename and create question file
        filename = message.split("@")[1]
        self.makeQuestionFile(filename)
        
        # Send question file
        try:
            # Check if the file exists
            if os.path.isfile(filename):
                # Open the file, read its contents and send it to the client
                file = open(filename, 'r')
                file_contents = file.read()
                TMsocket.send(file_contents.encode())
                print(f"[+] Question file '{filename}' sent successfully.")
            else:
                print("[-] Failed to send question file.")   
        except Exception as e:
            print(f"Error occurred: {str(e)}")
    
    # Send whether a student's answer is correct or not to TM
    def executeCheckAnswer(self, message, TMsocket):
        # Grade question
        type = message.split("@")[1]
        question = message.split("@")[2]
        answer = message.split("@")[3]
        isCorrect, output = self.gradeQuestion(type, question, answer)
        # Send response 'correct' or 'wrong'
        try:
            if (isCorrect):
                TMsocket.send(f"correct@{output}".encode())
                print("[+] Response 'correct' sent successfully.")
            else:
                TMsocket.send(f"wrong@{output}".encode())
                print("[+] Response 'wrong' sent successfully.")
        except Exception as e:
            print(f"Error occurred: {str(e)}")
           
    # Send the answer of a question to TM 
    def executeSendAnswer(self, message, TMsocket):
        # Find correct answer for given question
        type = message.split("@")[1]
        question = message.split("@")[2]
        correctAns = self.getAnswer(type, question, 0) # 0 for not getting image
        try:
            TMsocket.send(correctAns.encode())
            print(f"[+] Answer '{correctAns}' sent successfully.")
        except Exception as e:
            print(f"Error occurred: {str(e)}")
          
    # Send the answer of PCQ as an image to TM 
    def executeSendImage(self, message, TMsocket):
        type = message.split("@")[1]
        question = message.split("@")[2]
        imageData = self.getAnswer(type, question, 1) # 1 for getting image
        try:
            TMsocket.send(imageData)
            print(f"[+] Image answer sent successfully.")
        except Exception as e:
            print(f"Error occurred: {str(e)}")
        
    # Execute send operation depending on TM's message request
    def sendToTM(self, message, TMclient):
        # Categorise message depending on TM's request 
        messageType = message.split("@")[0]
        if messageType == "get_file":
            self.executeSendFile(message, TMclient)
        elif messageType == "check_answer":
            self.executeCheckAnswer(message, TMclient)
        elif messageType == "get_answer":
            self.executeSendAnswer(message, TMclient)
        elif messageType == "get_image":
            self.executeSendImage(message, TMclient)
        else:
            print("[!] Error occurred: invalid message.")
    
    # Print messages of what type of request recevied from TM
    def printReceivedMsg(self, message):
        # Categorise message depending on TM's request 
        messageType = message.split("@")[0]
        if messageType == "get_file":
            print("[+] Message 'get file' from TM received.")
        elif messageType == "check_answer":
            print("[+] Message 'check answer' from TM received.")
        elif messageType == "get_answer":
            print("[+] Message 'get answer' from TM received.")
        elif messageType == "get_image":
            print("[+] Message 'get image' from TM received.")
        else:
            print("[!] Error: message received was not understood.")
            
    # Receive TM acknowledgement for sent request
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
                
    # Delete question files and close sockets on termination
    def clearMemory(self, socket):
        # Remove question files
        for file in self.filesList:
            try:
                if os.path.isfile(file):
                    os.remove(file)
            except OSError as e:
                print(f"[!] Failed to delete file '{file}'.\n")
        print("\n[-] Removed all student's question files.")
        
        # Close the QB server socket
        socket.close()
        print("[-] QB server connection closed.")
    
    # Create and run QB server socket for TM client to connect
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
                messageType = message.split("@")[0]
                
                # Send acknowledgment for received data
                if message :
                    if messageType != "get_image" :
                        self.printReceivedMsg(message)
                        TMclient.send("ACK".encode())
                        print("[+] Message 'ACKNOWLEDGEMENT' sent successfully.")
                    
                # Perform the required send operation
                time.sleep(0.1) # to prevent simultaneous ACK and message send
                self.sendToTM(message, TMclient)

                # Receive TM acknowledgement for sent data
                if messageType != "get_image":
                    self.receiveACK(message, TMclient)
                    
                TMclient.close()
                print("----- Connection to " + TMaddress[0] + ':' + str(TMaddress[1]) + " closed -----")
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            self.clearMemory(QBserver)