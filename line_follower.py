import urllib.request
import cv2
import numpy as np
import socket

kp=1
ki=0
kd=0
integrate=0
last_error=0

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ip = socket.gethostbyname(socket.gethostname())
port = 1234
address = (ip,port)
server.bind(address)
server.listen(1)
print('Started listening to ',ip,':',port)
client,addr = server.accept()
print('Got connection from',addr[0],':',addr[1])

def motion_x(spd):
    msg = str(spd)+ "\r"
    msg_b=msg.encode()
    if(not client.send(msg_b)):
        return

url='http://192.168.1.104:8080/shot.jpg'

while(True):
    RawImg=urllib.request.urlopen(url) 
    imgNP=np.array(bytearray(RawImg.read()),dtype=np.uint8)
    img=cv2.imdecode(imgNP,-1)

    img=cv2.resize(img,(int(img.shape[1]/4),int(img.shape[0]/4)))
    blur_img = cv2.GaussianBlur(img, (5, 5), 0)

    height, width = img.shape[:2]

    
    cv2.imshow('IMAGE',img)       

    v=0
    for i in range(width):
        if(blur_img[height-1][i][0]< 50 and v==0):
            l=i
            v=1
        elif(blur_img[height-1][i][0]> 127 and v==1):
            r=width-i
            break

    error=(l-r)
    integrate=integrate+error
    if(integrate>=1000):
        integrate=1000
    elif(integrate<=-1000):
        integrate=-1000
    derivative=error-last_error
    last_error=error
    spd=kp*error+ki*integrate+kd*derivative
    print(spd)
    motion_x(spd)
    
    k = cv2.waitKey(3) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()
exit(0)
