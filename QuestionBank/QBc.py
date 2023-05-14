import csv
import os
import sys
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
            
    # Grade C PCQ
    def gradePCQ(self, question, student_answer):
        # Find the corressponding question
        with open(self.pcqcCSV, "r") as file:
            lines = file.readlines()
            for i in range(len(lines)):
                print(question.rstrip())
                print(lines[i].rstrip())
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
                        return False
                    
                    # Execute the code
                    process = subprocess.Popen(["./TFF"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                    stdout, stderr = process.communicate()

                    # Delete the file after the code is executed
                    try:
                        os.remove(os.path.abspath("tempTestFile.c"))
                        os.remove(os.path.abspath("TFF"))
                    except OSError:
                        pass
                    
                    # Check the output of the code
                    if (stdout.strip() == data[1].strip()):
                        return True
                    else:
                        print("[!] stderr:\t" + stderr.strip())
                        return False
        return False
    
    # Get PCQ answer from given question
    def getPCQanswer(self, question):
        with open(self.pcqcCSV, "r") as file:
            lines = file.readlines()
            for i in range(len(lines)):
                if question.rstrip() == lines[i].rstrip():
                    with open("./CQuestions/pcqcTests.txt", "r") as testData:
                        testData = testData.readlines()
                        data = testData[i].split("@")
                        print(data[1].strip())
                        return f"Input data:{data[0].strip()}, Expected output:{data[1].strip()}"
        return ""