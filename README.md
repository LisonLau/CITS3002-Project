# CITS3002 Networks Project

Students undertake a test using a standard web-browser. Their web-browser will communicate with a software application termed a Test-Manager (TM). The TM will communicate with software application termed Question-Bank (QB). TM manages the testing of students, while QB generates and assesses questions. 

## Dependencies
- [Python3](https://www.python.org/downloads/)


## Installation

1. Clone the repository:
```bash
git clone https://github.com/LisonLau CITS3002-Project.git
```
2. Download and install [Python3](https://www.python.org/downloads/).
3. Use the appropriate environment to run C applications such as Linux or macOS

## Usage

Instructions on how to run the project:
1. Navigate to the QuestionBank folder
2. Start the QB program by running the command below:
```bash
python3 QB.py <QB IP address>
```
3. Navigate to the TestManager folder
4. Compile the TM program
```bash
make
```
5. Start the TM program by running the command below:
```bash
./TM <TM IP address> <QB IP address>
```
6. Multiple student clients can access the test browser by a URL 
```bash
https://<TM IP address>:8080
```
- QB IP address: the IP address of the host device running the QuestionBank application
- TM IP address: the IP address of the host device running the TestManager application
