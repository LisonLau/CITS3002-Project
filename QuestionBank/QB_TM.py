import csv
import random
import socket
import time
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

    def makeQuestionFile(self, filename):
        # filename format : student_password
        question_list = self.getRandom()
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerows(question_list)
            
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
            answer = self.QBpyInstance.getPCQanswer(ques)
        return answer
    
    def categoriseMessage(self, message):
        if len(message.split("@")) == 2:
            return "get_file"
        elif len(message.split("@")) == 3:
            return "check"
        elif len(message.split("@")) == 4:
            return "get_ans"
        else:
            return ""
        
    def executeSendFile(self, message, TMsocket):
        # Get filename and create question file
        filename = message.split("@")[1]
        self.makeQuestionFile(filename)
        
        # Check if the file exists
        if os.path.isfile(filename):
            # Open the file and read its contents
            with open(filename, 'rb') as f:
                file_contents = f.read()

            # Send the file contents to the client
            TMsocket.sendall(file_contents)
            print(f"[+] Question file '{filename}' sent successfully.")
        else:
            print("[-] Failed to send question file.")   
    
    def executeCheckAnswer(self, message, TMsocket):
        # Grade question
        type, question, answer = message.split("@")
        isCorrect = self.gradeQuestion(type, question, answer)
        # Send response 'correct' or 'wrong'
        if (isCorrect):
            TMsocket.send("correct".encode())
            print("[+] Response 'correct' sent successfully.")
        else:
            TMsocket.send("wrong".encode())
            print("[+] Response 'wrong' sent successfully.")
            
    def executeSendAnswer(self, message, TMsocket):
        # Find correct answer for given question
        type = message.split("@")[2]
        question = message.split("@")[3]
        correctAns = self.getAnswer(type, question)
        TMsocket.send(correctAns.encode())
        print(f"[+] Answer '{correctAns}' sent successfully.")
        
    def sendToTM(self, message, TMclient):
        # Categorise message received as 'get' or 'check'
        if self.categoriseMessage(message) == "get_file":
            print("[+] Message 'get file' from TM received.")
            self.executeSendFile(message, TMclient)
        elif self.categoriseMessage(message) == "check":
            print("[+] Message 'check' from TM received.")
            self.executeCheckAnswer(message, TMclient)
        elif self.categoriseMessage(message) == "get_ans":
            print("[+] Message 'get answer' from TM received.")
            self.executeSendAnswer(message, TMclient)
        else:
            print("[!] Error: message received was not understood.", message)
    
    def runQBserver(self, clientNetworkIp):
        # hostname = socket.gethostname()
        # host = socket.gethostbyname(hostname)  
        host = clientNetworkIp
        port = 8888
        thread_count = 0
        
        # Create server socket
        QBserver = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print('[+] Server socket created.')
        
        # Set socket opt
        QBserver.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        print('[+] Set socket options successful.')
        
        # Bind socket to host and port
        try:
            QBserver.bind((host, port))
        except socket.error as e:  
            print(f'[-] {str(e)}')
            return 0
        print('[+] Binding successful.')

        # Listen for connections
        QBserver.listen(5)
        print(f"[*] Listening as {host}:{port}")
        
        try:
            while True:
                # Establish connection 
                TMclient, TMaddress = QBserver.accept()
                print('[+] Connected to: ' + TMaddress[0] + ':' + str(TMaddress[1]))
                
                # Receive a string message
                message = TMclient.recv(BUFFERSIZE).decode()
                
                # Perform the required send operation
                self.sendToTM(message, TMclient)

                # Receive acknowledgement for sent data
                # ack = TMclient.recv(BUFFERSIZE).decode()
                # if ack == "ACK":
                #     print("[+] Acknowledgement received for sent data.")
                # else:
                #     print("[-] Acknowledgement not received.")
                    
                TMclient.close()
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            QBserver.close()    # close QB socket
            print("[-] Connection closed.")