# CITS3002 Networks Project

Students undertake a test using a standard web-browser. Their web-browser will communicate with a software application termed a Test-Manager (TM). The TM will communicate with software application termed Question-Bank (QB). TM manages the testing of students, while QB generates and assesses questions. 

The web-browser and TM communicate through HTTP requests. Upon accessing the test, the TM sends a HTTP response, containing the HTML for the login page, and question pages to the web-browser. The web-browser interacts back by sending POST requests through submit buttons. TM receives the request containing the student input and passes them to QB for answer checking.

TM and QB communicate through requests and acknowledgements. Requests are sent as message strings. Both TM and QB exchange acknowledgements upon receiving requests from each other. A simple stop-and-wait protocol is represented in our implementation, where both ends wait up to 5 seconds if acknowledgements are not yet received. If the time limit is exceeded, the request is retransmitted.

TestManager accepts multiple connections from students. Students enter their IP addresses to access the TM. TM receives IP addresses for accessed students.  

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
```bash
cd QuestionBank
```
2. Start the QB program by running the command below:
```bash
python3 QB.py <QB IP address>
```
3. Navigate to the TestManager folder
```bash
cd TestManager
```
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

## References
[Decoding url] https://stackoverflow.com/questions/2673207/c-c-url-decode-library
[How to transfer files using TCP socket] https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-python3/
[How to transfer files using sockets in Python] https://www.thepythoncode.com/article/send-receive-files-using-sockets-python
[File transferring using sockets in C] https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/
