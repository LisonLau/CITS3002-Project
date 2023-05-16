# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import csv
import os
import subprocess

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
        # Find the corressponding question

        try:
            with open(self.pcqpyCSV, "r") as file:
                lines = file.readlines()
                for i in range(len(lines)):
                    if question.rstrip() == lines[i].rstrip():
                        # Find corresponding test from the pcqpy test file
                        with open("./PythonQuestions/pcqpyTests.txt", "r") as testData:
                            testData = testData.readlines()
                            data = testData[i].split("@")

                        # Write the python file to execute
                        with open("tempTestFile.py", "w") as temp:
                            temp.write(student_answer + "\n")
                            temp.write(data[0])

                        # Execute the python file with the student's code
                        result = ""
                        try:
                            result = subprocess.run(["python3", os.path.abspath("tempTestFile.py")], capture_output=True, text=True, timeout=2)
                            print(result.stdout.strip(),data[1].strip())
                        except subprocess.TimeoutExpired:
                            print(f"[!] Student's answer timed-out")

                        # Delete the file after the code is executed
                        try:
                            os.remove(os.path.abspath("tempTestFile.py"))
                        except OSError:
                            pass

                        # Check the output of the program
                        if (result):
                            if (result.stdout.strip() == data[1].strip()):
                                return True, result.stdout.strip()
                            else:
                                print(f'[!] stderr: {result.stderr.strip()}')
                                return False, result.stderr.strip().replace("\n", "<br>")
                        return False, "Error: TimeoutExpired"

            return False, "An internal QB error has occured."
        except Exception as e:
            print(f"Error occured: {str(e)}")      
        return False, ""
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        try:
            with open(self.pcqcCSV, "r") as file:
                lines = file.readlines()
                for i in range(len(lines)):
                    if question.rstrip() == lines[i].rstrip():
                        with open("./CQuestions/pcqcTests.txt", "r") as testData:
                            testData = testData.readlines()
                            data = testData[i].split("@")
                            print(data[1].strip())
                            return f"<br>Input data: {data[0].strip()}<br>Expected output: {data[1].strip()}"
            return "An internal QB error has occured."
        except Exception as e:
            print(f"Error occured: {str(e)}")      
        return ""
    
    # Get PCQ image answer from given question
    def getPCQimage(self, question):
        try:
            with open(self.pcqcCSV, "r") as file:
                lines = file.readlines()
                for i in range(len(lines)):
                    if question.rstrip() == lines[i].rstrip():
                        imagefile = f"./CQuestions/pcqc{i}.png"
                        image = open(imagefile, 'rb')
                        imageData = bytes(image.read())
                        image.close()
                        return imageData
        except Exception as e:
            print(f"Error occured: {str(e)}")      
        return ""

