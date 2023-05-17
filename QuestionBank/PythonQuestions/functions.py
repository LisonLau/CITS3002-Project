"""

    NOT FOR USE, JUST TO FILL IN STUDENT ANSWERS!!!!!

"""

def add_nums(num1,num2):
    return num1 + num2


def string_len(string):
    return len(string)


def sum_even(numlist):
    total = 0
    for num in numlist:
        if num % 2 == 0:
            total += num
    return total


def reverse_string(string):
    return string[::-1]


def longest_len(stringlist):
    longest = 0
    for string in stringlist:
        if (len(string) > longest):
            longest = len(string)
    return longest
