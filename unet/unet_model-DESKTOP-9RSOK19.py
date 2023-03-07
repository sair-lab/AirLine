""" Full assembly of the parts to form the complete network """

from .unet_parts import *

sig=nn.Sigmoid()
class UNet(nn.Module):
    def l2n(self,x):
        return torch.nn.functional.normalize(x, p=2.0, dim=1)

    def __init__(self, n_channels, n_classes, bilinear=False):
        super(UNet, self).__init__()
        self.IN=nn.InstanceNorm2d(1)
        self.n_channels = n_channels
        self.n_classes = n_classes
        self.bilinear = bilinear

        self.inc = DoubleConv(n_channels, 64)
        self.down1 = Down(64, 128)
        self.down2 = Down(128, 256)
        self.down3 = Down(256, 512)
        factor = 2 if bilinear else 1
        self.down4 = Down(512, 1024 // factor)
        self.up1 = Up(1024, 512 // factor, bilinear)
        self.up2 = Up(512, 256 // factor, bilinear)
        self.up3 = Up(256, 128 // factor, bilinear)
        self.up4 = Up(128, 64, bilinear)
        self.outc = OutConv(64, n_classes)

        self.c6=nn.Sequential(
            nn.Conv2d(in_channels=1024,out_channels=1024,kernel_size=5,padding=2,stride=1),
            nn.ReLU(),
            nn.ConvTranspose2d(in_channels=1024,out_channels=1024,kernel_size=5,padding=2,stride=1),
            nn.ReLU(),
        )

    def forward(self, x):
        #x=self.IN(x)
        x1 = self.l2n(self.inc(x))
        x2 = self.l2n(self.down1(x1))
        x3 = self.l2n(self.down2(x2))
        x4 = self.l2n(self.down3(x3))
        x5 = self.l2n(self.down4(x4))
        #x3=self.IN(x3)
        x5=self.c6(x5)
        x = self.l2n(self.up1(x5, x4))
        x = self.l2n(self.up2(x4, x3))
        x = self.l2n(self.up3(x3, x2))
        x = self.up4(x, x1)
        logits = self.outc(x)
        return logits#,logits[:,1:]

