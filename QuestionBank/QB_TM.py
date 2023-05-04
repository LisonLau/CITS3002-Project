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
            isCorrect = False
        elif type == "mcqpy":   # PYTHON multiple choice question
            isCorrect = self.QBpyInstance.gradeMCQ(ques, ans)
        elif type == "pcqpy":   # PYTHON programming challenge question
            isCorrect = False
        return isCorrect

    def getAnswer(self, type, ques):
        answer = ""
        if type == "mcqc":      # C multiple choice question
            answer = self.QBcInstance.getMCQanswer(ques)
        elif type == "pcqc":    # C programming challenge question
            answer = "NOT IMPLEMENTED YET"
        elif type == "mcqpy":   # PYTHON multiple choice question
            answer = self.QBpyInstance.getMCQanswer(ques)
        elif type == "pcqpy":   # PYTHON programming challenge question
            answer = "NOT IMPLEMENTED YET"
        return answer
    
    def categoriseMessage(self, message):
        if len(message.split(",")) == 2:
            return "get_file"
        elif len(message.split(",")) == 3:
            return "check"
        elif len(message.split(",")) == 4:
            return "get_ans"
        
    def executeSendFile(self, message, TMsocket):
        # Get filename and create question file
        filename = message.split(",")[1]
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
        type, question, answer = message.split(",")
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
        type = message.split(",")[2]
        question = message.split(",")[3]
        correctAns = self.getAnswer(type, question)
        TMsocket.send(correctAns.encode())
        print(f"[+] Answer '{correctAns}' sent successfully.")
    
    def runQBserver(self):
        # hostname = socket.gethostname()
        # host = socket.gethostbyname(hostname)  
        host = '127.0.0.1'
        port = 8888
        thread_count = 0
        
        # Create server socket
        QBsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print('[+] Server socket created.')
        
        # Set socket opt
        QBsocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        print('[+] Set socket options successful.')
        
        # Bind socket to host and port
        try:
            QBsocket.bind((host, port))
        except socket.error as e:  
            print(f'[-] {str(e)}')
            return 0
        print('[+] Binding successful.')

        # Listen for connections
        QBsocket.listen(5)
        print(f"[*] Listening as {host}:{port}")
        
        try:
            while True:
                # Establish connection 
                TMsocket, TMaddress = QBsocket.accept()
                print('[+] Connected to: ' + TMaddress[0] + ':' + str(TMaddress[1]))
                
                # Receive a string message
                message = TMsocket.recv(BUFFERSIZE).decode()
                
                # Categorise message received as 'get' or 'check'
                # Perform required send operations
                if self.categoriseMessage(message) == "get_file":
                    print("[+] Message 'get file' from TM received.")
                    self.executeSendFile(message, TMsocket)
                elif self.categoriseMessage(message) == "check":
                    print("[+] Message 'check' from TM received.")
                    self.executeCheckAnswer(message, TMsocket)
                elif self.categoriseMessage(message) == "get_ans":
                    print("[+] Message 'get answer' from TM received.")
                    self.executeSendAnswer(message, TMsocket)
                    
                    
                # Receive acknowledgement for sent data
                # ack = TMsocket.recv(BUFFERSIZE).decode()
                # if ack == "ACK":
                #     print("[+] Acknowledgement received for sent data.")
                # else:
                #     print("[-] Acknowledgement not received.")
                    
                TMsocket.close()
        # If the user presses Ctrl+C, close the connection and the socket
        except KeyboardInterrupt:
            QBsocket.close()    # close QB socket
            print("[-] Connection closed.")