import numpy as np
import matplotlib.pyplot as plt
%matplotlib inline
from PIL import Image


#filepath="./" #The current directory
#filename=""  Nombre del archivo

#pic=Image.open("/content/drive/My Drive/Colab Notebooks/oso1.jpg") #Ruta del archivo



def imagenArray(pic):
    pic_arr = np.asarray(pic) #convierte imagen a Array
    pic_arr.shape

    return pic_arr


def convertirImagen(Pic):
    
    #Convertir imagen a RED
    pic_red=pic_arr.copy()
    pic_red[:,:,1]=0
    pic_red[:,:,2]=0
    
    #guardarla


    #Convertir imagen a GREEN
    pic_green=pic_arr.copy()
    pic_green[:,:,0]=0
    pic_green[:,:,2]=0

    #Convertir imagen a BLUE
    pic_blue=pic_arr.copy()
    pic_blue[:,:,0]=0
    pic_blue[:,:,1]=0






