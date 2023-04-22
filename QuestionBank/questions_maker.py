import csv

from get_questions import getRandom

def makeQuestionFile(student, password):
    # filename format : student_password
    filename = student + password + ".csv"
    question_list = getRandom()
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(question_list)
    return filename