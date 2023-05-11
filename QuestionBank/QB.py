# Student 1: Allison Lau   (23123849)
# Student 2: Alicia Lau    (22955092)
# Student 3: Li-Anne Long  (23192171)

import sys
import ipaddress
from QB_TM import *

if __name__ == '__main__':
    # Check if there is an argument
    if len(sys.argv) != 2:
        print(f"Usage: python {sys.argv[0]} <QB IP address>")
        sys.exit(1)
    
    # Check if the argument is a valid IPv4 address
    SERVER_HOST = sys.argv[1]
    try:
        ipaddress.ip_address(SERVER_HOST)
    except ValueError:
        print(f"Error: Invalid QB IP address: {SERVER_HOST}")
        sys.exit(1)
    
    QB = QuestionBank(SERVER_HOST)
    QB.runQBserver()