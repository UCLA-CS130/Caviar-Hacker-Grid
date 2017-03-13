import time
import telnetlib
import subprocess

print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
time.sleep(1)
print("Welcome.")
time.sleep(2)
print("\nThis is a demo of the RSA Encryption Capabilities of this server.")
time.sleep(3)
print("\nRSA works based on the fact that is is really difficult to factor the products of large primes.")
time.sleep(5)
print("\nAs in, it could take millions of years to break 4096-bit RSA by brute strength using one of our laptops.")
time.sleep(5)
print("\nIn RSA there is a public key used to encrypt messages, and a private key to decrpyt the message. The public key is distributed, the private key is what's secret.")
time.sleep(5)
print("Here, we implement the two stages of RSA - key generation and the RSA Algorithm - to encrypt an HTTP request sent to the server.")
time.sleep(3)
print("\nBeginning Server")
time.sleep(2)

subprocess.Popen(['sh', './webserver', 'rsa_test_config'])
time.sleep(5)
req = 'GET /rsa HTTP/1.1\r\nHost: localhost:2020\r\n\r\n'
tn = telnetlib.Telnet('localhost', 2020)
tn.write(req.encode('ascii'))

