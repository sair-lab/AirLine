# AirLine - demo code
1. download unet and dexined [checkpoints](https://drive.google.com/drive/folders/1Fe6DD38mai1ahvKm3bsHdTwLJCI14KeO?usp=share_link) and put it in the checkpoints folder
2. specify if using Unet or Dexined using variable _usingUnet_
3. change the directory variable _sequencePath_ and run Airline_demo.py in any way you want

youtube link: https://youtu.be/EKDx3Z9qYUQ

note: This code may have issue running on MacOS given dependency of dll libraries

The directory should look like:

├──AirLine
    ├── checkpoints
    │   ├── unet.pth
    │   └── dexi.pth
    ├── datasets
    │   ├── Wireframe
    │   │   ├── gt
    │   │   │   ├── xxxxx.jpg
    │   │   │   └── xxxxx.jpg
    │   │   └── data
    │   │       ├── xxxxx.jpg
    │   │       └── xxxxx.jpg
    │   └── Wireframe
    │       ├── gt
    │       └── data
    ...
# Abstract
Xiao Lin, Chen Wang.
Line detection is widely used in many robotic tasks such as scene recognition, 3D reconstruction, and simultaneous localization and mapping (SLAM). Compared to points, lines can provide both low-level and high-level geometrical information for downstream tasks. In this paper, we propose a novel edge-based line detection algorithm, AirLine, which can be applied to various tasks. In contrast to existing learnable endpoint-based methods which are sensitive to the geometrical condition of environments, AirLine can extract line segments directly from edges, resulting in a better generalization ability for unseen environments. Also to balance efficiency and accuracy, we introduce a region-grow algorithm and local edge voting scheme for line parameterization. To the best of our knowledge, AirLine is one of the first learnable edge-based line detection methods. Our extensive experiments show that it retains state-of-the-art-level precision yet with a 3-80× runtime acceleration compared to other learning-based methods, which is critical for low-power robots.