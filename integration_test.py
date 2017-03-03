# This is an integration test which analyzes the server's
# ability to respond to a request made by curl
import subprocess
import os
import telnetlib
## Global Vars ##
passing = True
port = 2024;
#TODO: add a data strx to hold names of failed tests


## Formatting Functions ##
def line():
    print("----------------------------------------")
def nameTest(name):
    line()
    print("Testing: " + name)
def pas():
    print("PASSED\n")

## Start Server ##
TONULL = open(os.devnull, 'w')
serverProcess = subprocess.Popen(['./webserver', 'test_config'], stdout=TONULL)

## Testing Process ##
print("\nBeginning Integration Test.")

nameTest("Basic Connection using Echo")
#echoReq = 'GET /echo HTTP/1.1\r\nHost: localhost:2024\r\n\r\n'
#echoTest = telnetlib.Telnet('localhost', 2025, 2)
#echoTest.write(req2.encode('ascii'))
#echoResult = echoTest.read_all().decode('utf-8')
#if echoResult == echoReq:

command2 = 'curl -Is localhost:' + str(port) + " | cat"
output = subprocess.check_output(command2, shell=True)
if output != "":
    pas()
    print(output)
    line()
else:
    print("ERROR: Cannot connect to server.")
    passing = False

## Multithreading Test ##
req1 = 'GET /block HTTP/1.1\r\nHost: localhost:2025\r\n\r\n'
req2 = 'GET /echo HTTP/1.1\r\nHost: localhost:2025\r\n\r\n' 
nameTest('Multithreading')
print('Sending blocking request\n')
p = telnetlib.Telnet('localhost', 2025, 2)
p.write(req1.encode('ascii'))

print('Sending echo request after blocking request.\n')
q = telnetlib.Telnet('localhost', 2025, 2)
q.write(req2.encode('ascii'))
result = q.read_all().decode('utf-8')
print("Result of echo request:\n")
print(result)
line()
## Kill Server ##
print("\nKilling Server.\n")
serverProcess.kill()


## Conclusion ##
print("Passed all tests?\n" + str(passing) + '\n')
#if passing:
#    return True
#else:
#    return False
    

## Notes ##
# Todo: add/utilize an http response parser that checks for correct
# status codes, content type, etc.


