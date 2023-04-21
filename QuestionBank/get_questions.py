import csv
import random

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