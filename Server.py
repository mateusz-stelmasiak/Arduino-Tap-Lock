import socket

IPw = input("Podaj IP serwera: ")
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # utworzenie gniazda
serversocket.bind((IPw, 8070))  # dowiazanie do portu 1234
serversocket.listen(5)


def listentostatus():
    return str(clientSocket.recv(1024), 'utf-8')


def authorize(matchScore):
    if float(matchScore) > 0.5:
        print("success")
    else:
        print("Auth failed")


print("Nasluchiwanie: ")
while 1:
    clientSocket, address = serversocket.accept()
    rec = listentostatus()
    authorize(rec)
