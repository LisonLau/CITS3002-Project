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
        with open(self.pcqpyCSV, "r") as csvfile:
            reader = csv.reader(csvfile)
            for line in reader:
                type = "pcqpy"
                PCquestions.append([type, line])
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
        # Find the corressponding question
        with open(self.pcqpyCSV, "r") as file:
            lines = file.readlines()
            for i in range(len(lines)):
                if question.rstrip() == lines[i].rstrip():
                    # TODO: Issues with csv.... i want to cry
                    # Find corresponding test from the pcqpy test file
                    with open("./PythonQuestions/pcqpyTests.txt", "r") as testData:
                        testData = testData.readlines()
                        data = testData[i].split("|")

                    with open("tempTestFile.py", "w") as temp:
                        temp.write(student_answer + "\n")
                        temp.write(data[0])
                    result = subprocess.run(["python3", os.path.abspath("tempTestFile.py")], capture_output=True, text=True)
                    print(result.stdout.strip(),data[1].strip())

                    # Delete the file after the code is executed
                    try:
                        os.remove(os.path.abspath("tempTestFile.py"))
                    except OSError:
                        pass

                    if (result.stdout.strip() == data[1].strip()):
                        return True
        return False
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        with open(self.pcqpyCSV, "r") as file:
            lines = file.readlines()
            for i in range(len(lines)):
                if question.rstrip() == lines[i].rstrip():
                    with open("./PythonQuestions/pcqpyTests.txt", "r") as testData:
                        testData = testData.readlines()
                        data = testData[i].split("|")
                        print(data[1].strip())
                        return data[1].strip()
        return ""