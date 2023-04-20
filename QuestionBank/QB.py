import csv
import random

def getMCQ():
    MCquestions = []
    with open("mcq.csv", "r") as csvlines:
        lines = csv.reader(csvlines)
        for line in lines:
            MCquestions.append(line)
    return MCquestions
    
def getPCQ():
    PCquestions = []
    with open("pcq.csv", "r") as lines:
        for line in lines:
            PCquestions.append(line.rstrip())
    return PCquestions

def getRandom():
    allQuestions = getMCQ() + getPCQ()
    random.shuffle(allQuestions)
    randomQuestions = allQuestions[:10]
    return randomQuestions
        