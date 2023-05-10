# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

from QB_TM import *
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage:\tpython3 networkIp\n")
    QB = QuestionBank()
    QB.runQBserver(sys.argv[1])