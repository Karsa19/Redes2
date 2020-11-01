import os
import threading
import numpy as np
from PIL import Image

isfile = os.path.isfile
join = os.path.join

directory = "Practica1/imagenes"
number_of_files = sum(1 for item in os.listdir(directory) if isfile(join(directory, item)))



def imagenArray(pic):
    pic_arr = np.asarray(pic) #convierte imagen a Array
    pic_arr.shape

    return pic_arr


def convertirImagen(pic, i):

    pic_arr= imagenArray(pic)
    
    #Convertir imagen a RED
    pic_red=pic_arr.copy()
    pic_red[:,:,1]=0
    pic_red[:,:,2]=0

    #guardarla
    red= Image.fromarray(pic_red)
    red.save("Practica1/imagenes/"+ str(i) +"red.jpeg")


    #Convertir imagen a GREEN
    pic_green=pic_arr.copy()
    pic_green[:,:,0]=0
    pic_green[:,:,2]=0

    #guardarla
    green= Image.fromarray(pic_green)
    green.save("Practica1/imagenes/"+ str(i) +"green.jpeg")

    #Convertir imagen a BLUE
    pic_blue=pic_arr.copy()
    pic_blue[:,:,0]=0
    pic_blue[:,:,1]=0

    #guardarla
    blue= Image.fromarray(pic_blue)
    blue.save("Practica1/imagenes/"+ str(i) +"blue.jpeg")


for i in range(1,number_of_files + 1):
    pic=Image.open("Practica1/imagenes/" + str(i) +".jpeg")
    t = threading.Thread(name="hilo"+ str(i), target=convertirImagen(pic, i))







