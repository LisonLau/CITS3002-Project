import csv
import os

class QuestionBankC:
    # Initialise path names for C csv files
    def __init__(self):
        script_path = os.path.abspath(__file__) 
        path_list = script_path.split(os.sep)
        script_directory = path_list[0:len(path_list)-1]
        mcqc = "/CQuestions/mcqc.csv"
        pcqc = "/CQuestions/pcqc.csv"
        self.mcqcCSV = "/".join(script_directory) + "/" + mcqc
        self.pcqcCSV = "/".join(script_directory) + "/" + pcqc
        
    # Get and store C MCQ in a list
    def getMCQ(self):
        MCquestions = []
        with open(self.mcqcCSV, "r") as csvfile:
            reader = csv.reader(csvfile)
            for line in reader:
                type = "mcqc"
                line.insert(0, type)
                MCquestions.append(line)
        return MCquestions
    
    # Get and store C PCQ in a list
    def getPCQ(self):
        PCquestions = []
        with open(self.pcqcCSV, "r") as lines:
            for line in lines:
                type = "pcqc"
                PCquestions.append([type, line.rstrip()])
        return PCquestions
    
    # Grade C MCQ
    def gradeMCQ(self, question, student_answer):
        AllMCQ = self.getMCQ()
        print(AllMCQ)
        for mcq in AllMCQ:
            if mcq[1] == question and mcq[6] == student_answer:
                return True
        return False