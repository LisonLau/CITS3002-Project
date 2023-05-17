# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import csv
import os
import subprocess

# Question Bank Instance for C programming questions
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
            with open(self.pcqcCSV, "r") as file:
                lines = file.readlines()
                for i in range(len(lines)):
                    if question.rstrip() == lines[i].rstrip():
                        # Find corresponding test from the pcqc test file
                        with open("./CQuestions/pcqcTests.txt", "r") as testData:
                            testData = testData.readlines()
                            data = testData[i].split("@")

                        # Write the c file
                        with open("tempTestFile.c", "w") as temp:
                            temp.write("#include <string.h>\n#include <stdio.h>\n#include <stdlib.h>\n")
                            temp.write(student_answer + "\n")
                            temp.write("int main(int argc, char const *argv[]) {\n\t" + f"{data[0]}\n" + "\treturn 0;\n}")

                        # Compile the c file
                        result = subprocess.run(["cc","-std=c11","-Wall","-Werror","-o", "TFF", \
                                                    os.path.abspath("tempTestFile.c")], capture_output=True, text=True)
                        if result.stderr:
                            # If the c file with the student's answer doesn't compile
                            print("[!] stderr:\t" + result.stderr)

                            # Delete the uncompiled file
                            try:
                                os.remove(os.path.abspath("tempTestFile.c"))
                            except OSError:
                                pass
                            
                            return False, result.stderr.replace("\n", "<br>")
                        
                        # Execute the code
                        process = ""
                        try:
                            process = subprocess.run(["./TFF"], capture_output=True, text=True, timeout=2)
                        except subprocess.TimeoutExpired:
                            print(f"[!] Student's answer timed-out")
                        

                        # Delete the file after the code is executed
                        try:
                            os.remove(os.path.abspath("tempTestFile.c"))
                            os.remove(os.path.abspath("TFF"))
                        except OSError:
                            pass
                        
                        # Check the output of the code
                        print(process)
                        if (process):
                            if (process.stdout.strip() == data[1].strip()):
                                return True, process.stdout.strip()
                            else:
                                print("[!] stderr:\t" + process.stderr.strip())
                                return False, process.stderr.strip().replace("\n", "<br>")
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
                        imageFile = f"./CQuestions/pcqc{i}.png"
                        image = open(imageFile, 'rb')
                        imageData = bytes(image.read())
                        image.close()
                        return imageData, imageFile
        except Exception as e:
            print(f"Error occured: {str(e)}")      
        return ""