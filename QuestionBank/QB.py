import csv
import random
import socket

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
    allQuestions = getMCQ() + getPCQ()
    random.shuffle(allQuestions)
    randomQuestions = allQuestions[:10]
    return randomQuestions

def sendQuestions(student, password):
    # filename format : student_password
    filename = student + password + ".csv"
    question_list = getRandom()
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(question_list)
        
        
# def gradeMCQ():
# def gradePCQ():
    
        
        