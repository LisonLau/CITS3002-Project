import os

from get_questions import *
from questions_maker import *
from grade import *
from socket_transfer import *

sendQuestionFile("user1", "pass1")

# def gradePCQ(question, student_code):
#     AllPCQ = getPCQ()
#     idx = AllPCQ.index([question])
#     if idx == 0: