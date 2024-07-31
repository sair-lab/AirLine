# this file is yet to be furnished with comments and modifications

import timeit
import torch
import numpy as np
import cv2
from unet import *
import os
import CRG311 as crg
# import the C++ library for CRG and line parameterization

from deximodel import DexiNed
import timeit

#===orientation detector setting===
THETARESOLUTION=6
KERNEL_SIZE=9

usingUnet=0
# change this to 1 if you want to use unet, we use DexiNed for demo by default

if not usingUnet:
    Premodel=DexiNed().cuda(0)
    Premodel.load_state_dict(torch.load("checkpoints//dexi.pth"))
else:
    Premodel=UNet(1,1).cuda(0)
    Premodel.load_state_dict(torch.load('checkpoints//unet.pth'))


def init_OD(THETARESOLUTION,KERNEL_SIZE):
    OD=nn.Conv2d(1,THETARESOLUTION,KERNEL_SIZE,1,KERNEL_SIZE//2,bias=False).cuda()
    for i in range(THETARESOLUTION):
        kernel=np.zeros((KERNEL_SIZE,KERNEL_SIZE))
        angle=i*180/THETARESOLUTION
        x=(np.cos(angle/180*3.1415926)*50).astype(np.int32)
        y=(np.sin(angle/180*3.1415926)*50).astype(np.int32)
        
        cv2.line(kernel,(KERNEL_SIZE//2-x,KERNEL_SIZE//2-y),(KERNEL_SIZE//2+x,KERNEL_SIZE//2+y),1,1)
        OD.weight.data[i]=torch.tensor(kernel)
        cv2.imshow("k",cv2.resize(kernel,(100,100),interpolation=0))
    return OD

OrientationDetector=init_OD(THETARESOLUTION,KERNEL_SIZE)


if __name__ == '__main__':

    tempMem=np.zeros((50000,2),dtype=np.int32) #buffer needed for C++ program
    tempMem2=np.zeros((2,300000,2),dtype=np.int32) #buffer needed for C++ program

    sequencePath='D:\\v1.1\\v1.1\\test'
    sequencePath=r'D:\Downloads\new_euroc_style\new_euroc_style\cam1\data'
    #path='D:\Downloads\subt\handheld0\cam0\data'
    #path='fortest\\compare'
    'G:\OD\OneDrive - Georgia Institute of Technology\\1-codes\\17net\\fortest\\as'
    #path=r"D:\Downloads\handheld0\cam0\data\\"
    haveGT=0
    demo=1
    showDuplicate=0
    play=1000
    for o in os.listdir(sequencePath):
        linenum=0
        rx1=cv2.imread(sequencePath+'\\'+o,cv2.IMREAD_UNCHANGED)
        res=16
        dscale=1
        if demo:
            pass
        if not usingUnet:
            rx1=cv2.resize(rx1,(rx1.shape[1]//dscale//res*res,rx1.shape[0]//dscale//res*res))

        if len(rx1.shape)==2:
            rx1=cv2.cvtColor(rx1,cv2.COLOR_GRAY2RGB)
        elif rx1.shape[2]==3:
            pass
        elif rx1.shape[2]==4:
            rx1=cv2.cvtColor(rx1,cv2.COLOR_RGBA2RGB)
        rx1=np.ascontiguousarray(rx1)
        
        x1=rx1

        if usingUnet:
            x1=cv2.cvtColor(x1,cv2.COLOR_RGB2GRAY)

        t=timeit.default_timer()

        x1 = torch.tensor(x1).cuda()/255


        x1 = torch.tensor(x1).cuda()/255
        
        if usingUnet:
            x1=x1.unsqueeze(0)
        else:
            x1=x1.permute(2,0,1)
        lineDetection1=Premodel(x1.unsqueeze(0))
        THETADes=OrientationDetector(lineDetection1)
        imggradient=torch.cat([x1.unsqueeze(0)-x1.roll(1,1).unsqueeze(0),x1.unsqueeze(0)-x1.roll(0,1).unsqueeze(0)])
        THETADes=torch.nn.functional.normalize(THETADes-THETADes.mean(1), p=2.0, dim=1)


        edgeNp=lineDetection1.detach().cpu().numpy()[0,0]
        
        outMap=np.zeros_like(edgeNp,dtype=np.uint8)
        outMap=np.expand_dims(outMap,2).repeat(3,2)
        out=np.zeros((30000,2,2),dtype=np.float32)
        tempMem3=np.zeros((30000,2,2),dtype=np.float32)
        mask=lineDetection1[:,0]>0.1

        if usingUnet:
            if demo:
                edgeNp=(edgeNp>-2.5).astype(np.uint8)*255
            else:
                edgeNp=(edgeNp>-2.1).astype(np.uint8)*255
        else:
            if demo:
                edgeNp=(edgeNp>0.5).astype(np.uint8)*255#1.5
            else:
                edgeNp=(edgeNp>0.5).astype(np.uint8)*255
        cv2.imshow('edge',edgeNp)
        
        if not demo:
            linenum=crg.desGrow(outMap,edgeNp,THETADes[0].detach().cpu().numpy(),out,0.94,40,tempMem,tempMem2,tempMem3,THETARESOLUTION)#lineDetection1.detach().cpu().numpy()[0,0]
        else:
            linenum=crg.desGrow(outMap,edgeNp,THETADes[0].detach().cpu().numpy(),out,0.9,10,tempMem,tempMem2,tempMem3,THETARESOLUTION)#lineDetection1.detach().cpu().numpy()[0,0]




        cv2.imshow('seg',outMap)
        puredetection=np.zeros_like(rx1[:,:,0],dtype=np.uint8)

        rx1=cv2.cvtColor(rx1,cv2.COLOR_RGB2GRAY)
        rx1=cv2.cvtColor(rx1,cv2.COLOR_GRAY2RGB)

        rx2=rx1.copy()
        rx3=rx1.copy()
        
        print(linenum)



        out=(out).astype(np.int32)
        x1=x1.detach().cpu().numpy()*255
        x1=x1.astype(np.uint8)
        color=(80,157,233)
        color=(255,255,0)
        color3=(0,0,255)

        if demo:
            for i in range(linenum):
                length=np.sqrt((out[i,0,0]-out[i,1,0])**2+(out[i,0,1]-out[i,1,1])**2)
                if length>15:
                    pass
                    cv2.line(rx1,(out[i,0,1],out[i,0,0]),(out[i,1,1],out[i,1,0]),color,2)


        cv2.imshow("x",rx1)
        cv2.waitKey(1)