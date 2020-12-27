import time
import socket

Message = "Hallo"

# Open a new socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("169.254.133.121", 80))

s.send(str.encode(Message))
print("Transmit {} bytes...".format(len(Message)))

Response = s.recv(2048)
print(Response)
print(len(Response))

# Close the socket
s.close()
