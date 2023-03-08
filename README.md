# AirLine - demo code
simply change the directory variable _sequencePath_ and run Airline_demo.py in any way
youtube link: https://youtu.be/EKDx3Z9qYUQ
# !training code and checkpoint waiting to be uploaded!
may have issue running on MacOS given dependency of dll libraries

# Abstract
Xiao Lin, Chen Wang.
Line detection is widely used in many robotic tasks such as scene recognition, 3D reconstruction, and simultaneous localization and mapping (SLAM). Compared to points, lines can provide both low-level and high-level geometrical information for downstream tasks. In this paper, we propose a novel edge-based line detection algorithm, AirLine, which can be applied to various tasks. In contrast to existing learnable endpoint-based methods which are sensitive to the geometrical condition of environments, AirLine can extract line segments directly from edges, resulting in a better generalization ability for unseen environments. Also to balance efficiency and accuracy, we introduce a region-grow algorithm and local edge voting scheme for line parameterization. To the best of our knowledge, AirLine is one of the first learnable edge-based line detection methods. Our extensive experiments show that it retains state-of-the-art-level precision yet with a 3-80× runtime acceleration compared to other learning-based methods, which is critical for low-power robots.