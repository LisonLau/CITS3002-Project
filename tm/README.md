# Usage
1. Navigate to the Test Manager folder
```bash
cd tm
```
2. Compile the TM program
```bash
make
```
3. Start the TM program by running the command below:
```bash
./TM <TM IPv4 address> <QB IPv4 address>
```
4. Student clients can access the test via this URL: 
```bash
https://<TM IPv4 address>:8080
```
- QB IP address: the IP address of the host device running the QuestionBank application
- TM IP address: the IP address of the host device running the TestManager application
- PORTs are defaulted 8080 and 8888 for the server and client respectively
- PORTs can be customised in 'TM.h' file (lines 33 and 34)

## References / Citations
- [C Select Manual](https://man7.org/linux/man-pages/man2/select.2.html)
- [File Transfer using TCP Socket in C](https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/)
- [Decoding URL](https://stackoverflow.com/questions/2673207/c-c-url-decode-library)
- [JavaScript TAB to indent in textarea](https://stackoverflow.com/questions/6637341/use-tab-to-indent-in-textarea)