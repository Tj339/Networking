
from socket import *
import threading
import matplotlib.pyplot as plt
plt.style.use("seaborn")
plt.ion()

sentence = ''
serverName = '192.168.1.7'
serverPort = 12004

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))

i = 0
idx = []
tempArray = []

def graph():
    txt = open('file.txt', 'r')
    objt = txt.readline()
    global i
    global idx
    global tempArray
    idx.append(i)
    tempArray.append(float(objt))
    i = i + 1
  
    plt.xlim(left=max(0, i-10), right=i+10) #set the x-limits of the current axis 
    plt.ylim(0, 60) #set the y-limits of the current axis 
    plt.ylabel('Temperature (C)', fontname='Comic Sans MS', color='blue', fontsize=14) #set the label for the y-axis
    plt.xlabel('time (sec)', fontname='Comic Sans MS', color='blue', fontsize=14) #set the label for the y-axis
    
    plt.grid(True) #turn the grid on 
    plt.plot(idx, tempArray, color='r', linewidth=2) #plot idx versus temp - red line 
    plt.show() #display the figure
    plt.pause(.000001) #pause for interval seconds
    txt.close()
        
def file():
    while clientSocket.recv(1024):
        txt = open('file.txt', 'w')
        modifiedSentence = clientSocket.recv(1024)
        objt = float(modifiedSentence.decode())/100
        txt.write(str(objt))
        txt.close()
        graph()
        if sentence == "STOP":
            print('Disconnected')
            clientSocket.close()
            break
        
def task1():
    global sentence
    while True:
        sentence = input('Input:')
        if sentence == 'STOP':
            break;
        else:
            clientSocket.send(sentence.encode()) 

t1 = threading.Thread(target = task1) 

t1.start()
file()
