# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import csv
import os
import subprocess

# Question Bank Instance for Python programming questions
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
        try:
            with open(self.mcqpyCSV, "r") as csvfile:
                reader = csv.reader(csvfile)
                for line in reader:
                    type = "mcqpy"
                    line.insert(0, type)
                    MCquestions.append(line)
        except Exception as e:
            print(f"Error occured: {str(e)}")
        return MCquestions
    
    # Get and store C PCQ in a list
    def getPCQ(self):
        PCquestions = []
        # Open the Python PCQ csv file
        try:
            with open(self.pcqpyCSV, "r") as lines:
                for line in lines:
                    type = "pcqpy"
                    PCquestions.append([type, line.rstrip()])
        except Exception as e:
            print(f"Error occured: {str(e)}")
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
                                print(f"[!] Student's code had a stderr.")
                                return False, result.stderr.strip().replace("\n", "<br>")
                        return False, "Error: TimeoutExpired"
                      
            return False, "An internal QB error has occured."
        
        except Exception as e:
            print(f"Error occured: {str(e)}")      
        return False, ""
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        try: 
            with open(self.pcqpyCSV, "r") as file:
              lines = file.readlines()
              for i in range(len(lines)):
                  if question.rstrip() == lines[i].rstrip():
                      with open("./PythonQuestions/pcqpyTests.txt", "r") as testData:
                          testData = testData.readlines()
                          data = testData[i].split("@")
                          return f"<br>Input data:{data[0].strip()}<br>Expected output:{data[1].strip()}"
            return "An internal QB error has occured."
        except Exception as e:
            print(f"Error occured: {str(e)}")        
        return ""
    
    # Get PCQ image answer from given question
    def getPCQimage(self, question):
        try: 
            with open(self.pcqpyCSV, "r") as file:
                lines = file.readlines()
                for i in range(len(lines)):
                    if question.rstrip() == lines[i].rstrip():
                        imagefile = f"./PythonQuestions/pcqpy{i}.png"
                        image = open(imagefile, 'rb')
                        imageData = image.read()
                        image.close()
                        return imageData
        except Exception as e:
            print(f"Error occured: {str(e)}")
        return ""
