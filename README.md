# AirLine - using demo code
1. download unet and dexiNed [checkpoints](https://drive.google.com/drive/folders/11TQxJjpBoZVKcQ0Pmb6GH3h0qQk4CnNe?usp=sharing) and put it in the checkpoints folder
2. specify if using Unet or Dexined using variable _usingUnet_ in AirLine_demo.py, we recommand using dexiNed.
3. specify _sequencePath_ which should be a directory containing image sequence to test on.
3. run Airline_demo.py in any way you want.

youtube [demo video](https://youtu.be/EKDx3Z9qYUQ)

note: This code may have issue running on MacOS given dependency of dll libraries

# AirLine - run testing
*If you wish to run testing, download [test set](https://drive.google.com/drive/folders/1uprorwuIq8vOKYElQ27MLTw_f8W2sIoW?usp=sharing) and set up the directory which should look like:

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
        
then run AirLine-test.py
# Abstract
Xiao Lin, Chen Wang.
Line detection is widely used in many robotic tasks such as scene recognition, 3D reconstruction, and simultaneous localization and mapping (SLAM). Compared to points, lines can provide both low-level and high-level geometrical information for downstream tasks. In this paper, we propose a novel edge-based line detection algorithm, AirLine, which can be applied to various tasks. In contrast to existing learnable endpoint-based methods which are sensitive to the geometrical condition of environments, AirLine can extract line segments directly from edges, resulting in a better generalization ability for unseen environments. Also to balance efficiency and accuracy, we introduce a region-grow algorithm and local edge voting scheme for line parameterization. To the best of our knowledge, AirLine is one of the first learnable edge-based line detection methods. Our extensive experiments show that it retains state-of-the-art-level precision yet with a 3-80× runtime acceleration compared to other learning-based methods, which is critical for low-power robots.