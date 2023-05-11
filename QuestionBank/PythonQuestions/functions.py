"""

    NOT FOR USE, JUST TO FILL IN STUDENT ANSWERS!!!!!

"""

def reverse_string(string):
    return string[::-1]


def longest_len(stringlist):
    longest = 0
    for string in stringlist:
        if (len(string) > longest):
            longest = len(string)
    return longest

def string_len(string):
    return len(string)