from get_questions import getMCQ

def gradeMCQ(question, student_answer):
    AllMCQ = getMCQ()
    for mcq in AllMCQ:
        if mcq[0] == question and mcq[5] == student_answer:
            return True
    return False