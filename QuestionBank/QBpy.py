# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import csv
import os

class QuestionBankPython:
    # Initialise path names for Python csv files
    def __init__(self):
        script_path = os.path.abspath(__file__) 
        path_list = script_path.split(os.sep)
        script_directory = path_list[0:len(path_list)-1]
        mcqpy = "/PythonQuestions/mcqpy.csv"
        pcqpy = "/PythonQuestions/pcqpy.csv"
        self.mcqpyCSV = "/".join(script_directory) + "/" + mcqpy
        self.pcqpyCSV = "/".join(script_directory) + "/" + pcqpy
       
    # Get and store Python MCQ in a list 
    def getMCQ(self):
        MCquestions = []
        # Open the Python MCQ csv file
        with open(self.mcqpyCSV, "r") as csvfile:
            reader = csv.reader(csvfile)
            for line in reader:
                type = "mcqpy"
                line.insert(0, type)
                MCquestions.append(line)
        return MCquestions
    
    # Get and store C PCQ in a list
    def getPCQ(self):
        PCquestions = []
        # Open the Python PCQ csv file
        with open(self.pcqpyCSV, "r") as lines:
            for line in lines:
                type = "pcqpy"
                PCquestions.append([type, line.rstrip()])
        return PCquestions

    # Grade Python MCQ
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
    
    # Grade Python PCQ
    def gradePCQ(self, question, student_answer):
        pass
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        pass