""" Full assembly of the parts to form the complete network """

from .unet_parts import *

sig=nn.Sigmoid()
class UNet(nn.Module):
    def l2n(self,x):
        return torch.nn.functional.normalize((x), p=2.0, dim=1)

    def __init__(self, n_channels, n_classes, bilinear=False):
        super(UNet, self).__init__()
        self.IN=nn.InstanceNorm2d(1)
        self.n_channels = n_channels
        self.n_classes = n_classes
        self.bilinear = bilinear
        NUM=1

        self.inc = DoubleConv(n_channels, 64//NUM)
        self.down1 = Down(64//NUM, 128//NUM)
        self.down2 = Down(128//NUM, 256//NUM)
        self.down3 = Down(256//NUM, 512//NUM)
        factor = 1 if bilinear else 1
        self.down4 = Down(512//NUM, 1024 // factor//NUM)
        self.down5 = Down(1024//NUM, 2048 // factor//NUM)
        self.up0 = Up(2048//NUM, 1024 // factor//NUM, bilinear)
        self.up1 = Up(1024//NUM, 512 // factor//NUM, bilinear)
        self.up2 = Up(512//NUM, 256 // factor//NUM, bilinear)
        self.up3 = Up(256//NUM, 128 // factor//NUM, bilinear)
        self.up4 = Up(128//NUM, 64// factor//NUM, bilinear)
        self.outc = OutConv(64//NUM, n_classes)
        

        

    def forward(self, x):
        x=self.IN(x)
        x1 = self.inc(x)
        x2 = self.down1(x1)
        x3 = self.down2(x2)
        x4 = self.down3(x3)
        x5 = self.down4(x4)
        x6 = self.down5(x5)
        x=self.IN(x6)
        #x5=self.c6(x5)
        x = self.up0(x6, x5)
        x = self.up1(x, x4)
        x = self.up2(x, x3)
        x = self.up3(x, x2)
        x = self.up4(x, x1)
        logits = self.outc(x)
        return logits#,logits[:,1:]

class UNetT(nn.Module):
    def l2n(self,x):
        return torch.nn.functional.normalize(x, p=2.0, dim=1)

    def __init__(self, n_channels, n_classes, bilinear=False):
        super(UNetT, self).__init__()
        self.IN=nn.InstanceNorm2d(1)
        self.n_channels = n_channels
        self.n_classes = n_classes
        self.bilinear = bilinear
        NUM=4

        self.inc = DoubleConv(n_channels, 64//NUM)
        self.down1 = Down(64//NUM, 128//NUM)
        self.down2 = Down(128//NUM, 256//NUM)
        self.down3 = Down(256//NUM, 512//NUM)
        factor = 2 if bilinear else 1
        self.down4 = Down(512//NUM, 1024 // factor//NUM)
        self.up1 = Up(1024//NUM, 512 // factor//NUM, bilinear)
        self.up2 = Up(512//NUM, 256 // factor//NUM, bilinear)
        self.up3 = Up(256//NUM, 128 // factor//NUM, bilinear)
        self.up4 = Up(128//NUM, 64//NUM, bilinear)
        self.outc = OutConv(64//NUM, n_classes)
        

        

    def forward(self, x):
        x=self.IN(x)
        x1 = self.l2n(self.inc(x))
        x2 = self.l2n(self.down1(x1))
        x3 = self.l2n(self.down2(x2))
        x3=self.IN(x3)
        #x5=self.c6(x5)
        x = self.l2n(self.up3(x3, x2))
        x = self.up4(x, x1)
        logits = self.outc(x)
        return logits#,logits[:,1:]

