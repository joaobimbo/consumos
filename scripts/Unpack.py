#Receber os dados e guardar em variável
import socket
import struct
import numpy as np
from numpy.fft import fft
import matplotlib.pyplot as plt
from IPython import embed



def criar_socket(UDP_IP,UDP_PORT):
    #UDP_IP = "0.0.0.0"
    #UDP_PORT = 8543 #usar o mesmo port que o que está a enviar

    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
    sock.bind((UDP_IP, UDP_PORT))
    return(sock)
    
    
def receber_do_sock(sock):
    data, addr = sock.recvfrom(2048*8) # buffer 5*2048 bytes
    dn=np.frombuffer(data)    
    #dn=struct.unpack('f', data)
        
    return(dn)

def acrescentar(dados,dados_novos):
    #print('acrescentar')
    #print('tamanhos: %d %d %d %d %d'  % (len(dados),len(dados[1:2048*7]),len(dados[2049:2048*8]),len(dados[2048*7:2048*8]),len(dados_novos)))
    dados[1:2048*7]=dados[2049:2048*8]
    dados[2048*7:2048*8]=dados_novos
    return(dados)

def processar(dados):
    print('fazer fft')
    FFT_Dados=fft(dados)
    return np.abs(FFT_Dados)

def grafico(f):
    x = np.linspace(0, 2 * np.pi, 2048*8)
    #embed()
    plt.stem(x,f)
    plt.show(block=True)
        
def main():
    dados_obtidos = []
    dados=np.zeros(2048*8,dtype=float)

    sock=criar_socket("127.0.0.1",8543)
    print("Bora %d" % (len(dados)))
    while True:
        try:             
            dn=receber_do_sock(sock)
            #print(dn)
            dados=acrescentar(dados,dn)
            #dados_obtidos.append(dn)
            f=processar(dados)
            print('FFT')
            print(f)         
            grafico(f)
        except: 
            continue
    

    #data=b'\x00P\x03E\x00\x00\xfaD\x00\x00\x00\x00\x00@\xfbD\x00`\xfbD\x00@\x03Ed\x00\x00\x00e\x00\x00\x00n\x00\x00\x00o\x00\x00\x00'

    #Matriz_dados=struct.unpack('ffffffiiii', data)
      
main()  
    
#For unpacking, you just have to unpack a bit at a time:
#(i,), data = struct.unpack("I", data[:4]), data[4:]
#s, data = data[:i], data[i:]
