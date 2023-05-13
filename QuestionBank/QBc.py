# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

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
        # Open the C MCQ csv file
        try:
            with open(self.mcqcCSV, "r") as csvfile:
                reader = csv.reader(csvfile)
                for line in reader:
                    type = "mcqc"
                    line.insert(0, type)
                    MCquestions.append(line)
        except Exception as e:
            print(f"Error occured: {str(e)}")
        return MCquestions
    
    # Get and store C PCQ in a list
    def getPCQ(self):
        PCquestions = []
        # Open the C PCQ csv file
        try:
            with open(self.pcqcCSV, "r") as lines:
                for line in lines:
                    type = "pcqc"
                    PCquestions.append([type, line.rstrip()])
        except Exception as e:
            print(f"Error occured: {str(e)}")
        return PCquestions
    
    # Grade C MCQ
    def gradeMCQ(self, question, student_answer):
        AllMCQ = self.getMCQ()
        for mcq in AllMCQ:
            # Check if the question and student answer match an MCQ question in the list
            if mcq[1] == question and mcq[6] == student_answer:
                return True
        return False
    
    # Get MCQ answer from given question
    def getMCQanswer(self, question):
        AllMCQ = self.getMCQ()
        for mcq in AllMCQ:
            # Check if the question matches an MCQ question in the list
            if mcq[1] == question:
                return mcq[6]
            
    # Grade C PCQ
    def gradePCQ(self, question, student_answer):
        pass
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        pass