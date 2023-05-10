import csv
import os
import sys
import subprocess

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
        with open(self.pcqpyCSV, "r") as lines:
            for line in lines:
                line = line.split("|")
                type = "pcqpy"
                PCquestions.append([type, line[0].rstrip()])
        return PCquestions

    # Grade Python MCQ
    def gradeMCQ(self, question, student_answer):
        AllMCQ = self.getMCQ()
        for mcq in AllMCQ:
            if mcq[1] == question and mcq[6] == student_answer:
                return True
        return False
    
    # Get MCQ answer from given question
    def getMCQanswer(self, question):
        AllMCQ = self.getMCQ()
        for mcq in AllMCQ:
            if mcq[1] == question:
                return mcq[6]
    
    # Grade Python PCQ
    def gradePCQ(self, question, student_answer):
        print(question, student_answer)
        # Find the corressponding question
        functionCall
        expectedOutput
        with open(self.pcqpyCSV, "r") as lines:
            for line in lines:
                line = line.split("|")
                if question == line[0]:
                    functionCall = line[1]
                    expectedOutput = line[2]
                    break

        # subprocess.run(["python3", os.path.abspath(tempTestFile.py)])
        result = subprocess.run([sys.executable, "-c", student_answer], capture_output=True, text=True)

        if (result.stdout == expectedOutput):
            return True
        return False
        # pass
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        pass