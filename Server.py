import socket
#curr IP 192.168.137.17
IPw = input("Set server IP: ")
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # utworzenie gniazda
serversocket.bind((IPw, 8070))
serversocket.listen(5)
savedPasswords = []


def listen_to_status():
    return str(clientSocket.recv(2048), 'utf-8')


def save_password_to_server(password: str):
    try:
        savedPasswords.append(password)
        return "Success"
    except:
        return "Something went wrong"


def check_password(password: str):
    for passw in savedPasswords:
        if passw == password:
            return "Success"
    return "Wrong password"


def authorize(matchScore):
    if float(matchScore) > 0.5:
        print("Auth success")
    else:
        print("Auth failed")


def set_client_mode_new_password(clientSocket):
    print("sending...")
    set_password_as_bytes = str.encode("setPassword")
    clientSocket.send(set_password_as_bytes)


def decode_message(message):
    if message[0] == "%":
        authorize(message[1:])
    if message[0] == "$":
        save_password_to_server(message)
    else:
        print("Received: "+message)


def interpret_command(command,clientSocket):
    if command == "n":
        set_client_mode_new_password(clientSocket)



print("Listening for connections...")
# print("commands: n- new password|")
while 1:
    clientSocket, address = serversocket.accept()
    rec = listen_to_status()
    decode_message(rec)
