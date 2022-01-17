import socket

IPw = input("Podaj IP serwera: ")
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # utworzenie gniazda
serversocket.bind((IPw, 8070))  # dowiazanie do portu 1234
serversocket.listen(5)
savedPasswords = []


def listentostatus():
    return str(clientSocket.recv(2048), 'utf-8')


def savepasswordtoserver(password: str):
    try:
        savedPasswords.append(password)
        return "Success"
    except:
        return "Something went wrong"


def checkPassword(password: str):
    for passw in savedPasswords:
        if passw == password:
            return "Success"
    return "Wrong password"


def authorize(matchScore):
    if float(matchScore) > 0.5:
        print("success")
    else:
        print("Auth failed")


def setNewPassword(clientSocket):
    clientSocket.send("setPassword")


def decodeMessage(message):
    print(message)
    if message[0] == "%":
        authorize(message[1:])
    else:
        print(message)


print("Nasluchiwanie: ")
while 1:
    clientSocket, address = serversocket.accept()
    rec = listentostatus()
    decodeMessage(rec)
