import csv
import random
import socket
import os

def getMCQ():
    MCquestions = []
    with open("mcq.csv", "r") as csvfile:
        reader = csv.reader(csvfile)
        for line in reader:
            MCquestions.append(line)
    return MCquestions
    
def getPCQ():
    PCquestions = []
    with open("pcq.csv", "r") as lines:
        for line in lines:
            PCquestions.append([line.rstrip()])
    return PCquestions

def getRandom():
    # Not including the answers for MCQ
    allQuestions = [x[:-1] for x in getMCQ()] + getPCQ()
    random.shuffle(allQuestions)
    randomQuestions = allQuestions[:10]
    return randomQuestions

def makeQuestionFile(student, password):
    # filename format : student_password
    filename = student + password + ".csv"
    question_list = getRandom()
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(question_list)
    return filename
        
def gradeMCQ(question, student_answer):
    AllMCQ = getMCQ()
    for mcq in AllMCQ:
        if mcq[0] == question and mcq[5] == student_answer:
            return True
    return False

def sendQuestionFile(student, password):
    host = "127.0.0.1"  # host
    port = 8080         # port
    try:
        # create the client socket
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print(f"[+] Connecting to {host}:{port}.")
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
    except Exception as e:
        print("[-] Error occurred.")
    

sendQuestionFile("user1", "pass1")

# def gradePCQ(question, student_code):
#     AllPCQ = getPCQ()
#     idx = AllPCQ.index([question])
#     if idx == 0:
        

    
    
    

        