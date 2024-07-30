
from timeit import timeit
import torch
from unet import *
import CRG.extractC.CRG as crg
import numpy as np
import cv2
import os

from deximodel import DexiNed
import timeit



THETARESOLUTION=6#6
KERNEL_SIZE=9#7


def buildOrientationDetector():
    thetaN=nn.Conv2d(1,THETARESOLUTION,KERNEL_SIZE,1,KERNEL_SIZE//2,bias=False).cuda()
    for i in range(THETARESOLUTION):
        kernel=np.zeros((KERNEL_SIZE,KERNEL_SIZE))
        angle=i*180/THETARESOLUTION
        x=(np.cos(angle/180*3.1415926)*50).astype(np.int32)
        y=(np.sin(angle/180*3.1415926)*50).astype(np.int32)
        
        cv2.line(kernel,(KERNEL_SIZE//2-x,KERNEL_SIZE//2-y),(KERNEL_SIZE//2+x,KERNEL_SIZE//2+y),1,1)
        #kernel=(kernel+cv2.GaussianBlur(kernel,(7,7),cv2.BORDER_DEFAULT)*2-0.5)
        thetaN.weight.data[i]=torch.tensor(kernel)
    return thetaN

OrientationDetector=buildOrientationDetector()


LP0sum=0
LP1sum=0
LP2sum=0
LP3sum=0
LP5sum=0
LP10sum=0

if __name__ == '__main__':
    

    # =======================================================================================================================================

    tempMem=np.zeros((50000,2),dtype=np.int32)
    tempMem2=np.zeros((2,300000,2),dtype=np.int32)
    num=0
    allTime=0.0
    test=0.0
    allLength=0.0

    dataset="wireframe"
    dataset="yorkurban"

    if dataset=="yorkurban":
        path='Datasets\\YorkUrban'
        edgeDetector=DexiNed().cuda(0)
        edgeDetector.load_state_dict(torch.load('dexi.pth'))
        usingUnet=0
        config={
            "edgeThresh":0,
            "simThresh":0.8,
            "pixelNumThresh":10,
        }
    elif dataset=="wireframe":
        path='Datasets\\Wireframe'
        edgeDetector=UNet(1,1).cuda(0)
        edgeDetector.load_state_dict(torch.load('checkpoints//unet.pth'))
        usingUnet=1
        config={
            "edgeThresh":-2.5,
            "simThresh":0.7,
            "pixelNumThresh":10,
        }
    else:
        raise Exception("dataset not specified!")

    showDuplicate=0

    dataPath=path+'\\data'
    gtPath=path+'\\gt'
    for o in os.listdir(dataPath):
        num+=1
        rawLineNum=0
  
        rx1=cv2.imread(dataPath+"\\"+o,cv2.IMREAD_UNCHANGED)
        res=16
        dscale=1
        if not usingUnet:
            rx1=cv2.resize(rx1,(rx1.shape[1]//dscale//res*res,rx1.shape[0]//dscale//res*res))

        if len(rx1.shape)==2:
            rx1=cv2.cvtColor(rx1,cv2.COLOR_GRAY2RGB)
        elif rx1.shape[2]==3:
            pass
            #rx1=cv2.cvtColor(rx1,cv2.COLOR_RGB2GRAY)
        elif rx1.shape[2]==4:
            rx1=cv2.cvtColor(rx1,cv2.COLOR_RGBA2RGB)
        rx1=np.ascontiguousarray(rx1)
        
        x1=rx1

        if usingUnet:
            x1=cv2.cvtColor(x1,cv2.COLOR_RGB2GRAY)

        t=timeit.default_timer()

        x1 = torch.tensor(x1).cuda()/255

        if dataset=="yorkurban":
            o=o[:-3]+"bmp"
        gtimg=cv2.imread(gtPath+"\\"+o,cv2.IMREAD_GRAYSCALE)



        x1 = torch.tensor(x1).cuda()/255
        
        if usingUnet:
            x1=x1.unsqueeze(0)
        else:
            x1=x1.permute(2,0,1)
        edgeDetection=edgeDetector(x1.unsqueeze(0))
        ODes=OrientationDetector(edgeDetection)
        ODes=torch.nn.functional.normalize(ODes-ODes.mean(1), p=2.0, dim=1)
        edgeNp=edgeDetection.detach().cpu().numpy()[0,0]
        
        #edgeNp=lineDetection1
        outMap=np.zeros_like(edgeNp,dtype=np.uint8)
        outMap=np.expand_dims(outMap,2).repeat(3,2)
        out=np.zeros((3000,2,3),dtype=np.float32)
        tempMem3=np.zeros((3000,2,2),dtype=np.float32)
        
        
        edgeNp=(edgeNp>config["edgeThresh"]).astype(np.uint8)*255

        cv2.imshow('edge',edgeNp)

        
        rawLineNum=crg.desGrow(outMap,edgeNp,ODes[0].detach().cpu().numpy(),out,config["simThresh"],config["pixelNumThresh"],tempMem,tempMem2,tempMem3,THETARESOLUTION)# in this model and configuration, we use 0.7 as the threshold


        cv2.imshow('seg',outMap)# display the segmentation result
        puredetection=np.zeros_like(rx1[:,:,0],dtype=np.uint8)

        out=(out).astype(np.int32)

        actualLineNum=0

        for i in range(rawLineNum):
            length=np.sqrt((out[i,0,0]-out[i,1,0])**2+(out[i,0,1]-out[i,1,1])**2)
            actualLineNum+=1
            cv2.line(rx1,(out[i,0,1],out[i,0,0]),(out[i,1,1],out[i,1,0]),(0,0,255),2)
            if showDuplicate:
                duplicate=np.zeros_like(rx1[:,:,0],dtype=np.uint8)
                cv2.line(duplicate,(out[i,0,1],out[i,0,0]),(out[i,1,1],out[i,1,0]),255,2)
                rx1[(puredetection*duplicate)>0]=np.array([0,0,255],np.uint8)
            cv2.line(puredetection,(out[i,0,1],out[i,0,0]),(out[i,1,1],out[i,1,0]),255,1)
            pass
        
        
        
        #evaluate the result

        LP0=(puredetection>2)*1
        LP0=(LP0*(gtimg>1)).sum()/(gtimg>1).sum()

        LP1=(cv2.GaussianBlur(puredetection,(3,3),0)>2)*1
        LP1=(LP1*(gtimg>1)).sum()/(gtimg>1).sum()

        LP2=(cv2.GaussianBlur(puredetection,(5,5),0)>2)*1
        LP2=(LP2*(gtimg>1)).sum()/(gtimg>1).sum()

        LP3=(cv2.GaussianBlur(puredetection,(7,7),0)>2)*1
        LP3=(LP3*(gtimg>1)).sum()/(gtimg>1).sum()

        LP5=(cv2.GaussianBlur(puredetection,(11,11),0)>2)*1
        LP5=(LP5*(gtimg>1)).sum()/(gtimg>1).sum()

        LP10=(cv2.GaussianBlur(puredetection,(21,21),0)>2)*1
        LP10=(LP10*(gtimg>1)).sum()/(gtimg>1).sum()

        LP0sum+=LP0
        LP1sum+=LP1
        LP2sum+=LP2
        LP3sum+=LP3
        LP5sum+=LP5
        LP10sum+=LP10
        print(LP0,LP1,LP2,LP3,LP5,LP10)
    
                

        cv2.imshow("x",rx1)
               

        cv2.waitKey(1)
LP0=LP0sum/num
LP1=LP1sum/num
LP2=LP2sum/num
LP3=LP3sum/num
LP5=LP5sum/num
LP10=LP10sum/num

print("LP0:",LP0)
print("LP1:",LP1)
print("LP2:",LP2)
print("LP3:",LP3)
print("LP5:",LP5)
print("LP10:",LP10)