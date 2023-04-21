def PCQ1(x, y):
    return x + y
        
def PCQ2(s):
    return len(s)

def PCQ3(l):
    count = 0
    for i in l:
        if (i % 2 == 0):
            count += i
    return count

def PCQ4(s):
    return s[::-1]

def PCQ5(l):
    count = 0
    for i in l:
        if len(i) > count:
            count = len(i)
    return count