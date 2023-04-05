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
        │   ├── unet.pth
        │   └── dexi.pth
        ├── datasets
        │   ├── Wireframe
        │   │   ├── gt
        │   │   │   ├── xxxxx.jpg
        │   │   │   └── xxxxx.jpg
        │   │   └── data
        │   │       ├── xxxxx.jpg
        │   │       └── xxxxx.jpg
        │   └── YorkUrban
        │       ├── gt
        │       │   ├── xxxxx.jpg
        │       │   └── xxxxx.jpg
        │       └── data
        │           ├── xxxxx.jpg
        │           └── xxxxx.jpg
        ...

after specifying variable _dataset_ to be "Wireframe" or "YorkUrban" and then run AirLine-test.py.

# Abstract

Line detection is widely used in many robotic tasks such as scene recognition, 3D reconstruction, and simultaneous localization and mapping (SLAM). Compared to points, lines can provide both low-level and high-level geometrical information for downstream tasks. In this paper, we propose a novel edge-based line detection algorithm, AirLine, which can be applied to various tasks. In contrast to existing learnable endpoint-based methods which are sensitive to the geometrical condition of environments, AirLine can extract line segments directly from edges, resulting in a better generalization ability for unseen environments. Also to balance efficiency and accuracy, we introduce a region-grow algorithm and local edge voting scheme for line parameterization. To the best of our knowledge, AirLine is one of the first learnable edge-based line detection methods. Our extensive experiments show that it retains state-of-the-art-level precision yet with a $3-80\times$ runtime acceleration compared to other learning-based methods, which is critical for low-power robots.

<figure>
    <figcaption>
        The figure below provides a qualitative comparison across difference methods on untrained images with overlap detection highlighted in red .The first row is a distant view of city and the second row is street view, while the third to fifth rows are from UMA-VI dataset of subterranean environments.
    </figcaption>
    <img src="/img/cp2.png" />
    <figcaption>
        This gif showcases thenstability and great generalization ability of AirLine compared to LSD. In the first screenshot, both LSD and AirLine captured the line circled in red, while LSD lost track in the following frames, which is a good evidence of Airline's stability and robustness.
    </figcaption>
    <img src="/img/vsLSD.gif" />
    <figcaption>
        The following video demonstrates a qualitative comparison across difference methods on untrained sequence from UMA_VI. We found that the AirLine and LSD methods performed better than the LETR and LCNN in generating detailed line detections.
        Moreover, AirLine is preferred over LSD since it is able to merge the two sides of thin lines, while LSD tends to detect both sides.
        Though LCNN produces lines of high precision, it is not suitable for tasks that require detailed line inputs, as it tends to ignore short lines and connect two unrelated endpoints in complex environments. LETR, which performed the best under the endpoint precision mentioned in its original paper, was found to have the worst line quality in terms of precision, stability, and reasonable geometry. 
    </figcaption>
    <img src="/img/qualitativecomp.gif" />
    
</figure>

# Methodology

</figure>
    <img src="/img/pipeline.png" />
    <figcaption>
        A figure of Airline's pipeline
    </figcaption>
</figure>

As shown above, AirLine is a learnable edge-based line detection architecture that is composed of four modules including edge detection, orientation detection, conditional region-grow, and line parameterization. We next present their motivation and detailed process, respectively. We made a video to demonstrate each stage.

<figure>
    <img src="/img/pipeline.gif"/>
</figure>
By using edge-based learnable line detection approaches, AirLine has achieved state-of-art-level precision and significant efficiency and runtime boost.
<figure>
    <img src="/img/wffig.png" width=48%><img src="/img/yufig.png" width=48%/>
</figure>


## Watch Video

[<img src="https://user-images.githubusercontent.com/8695500/230144893-ece19d0f-77bf-431b-8cb4-605ff2d897eb.png" width="40%">](https://youtu.be/EKDx3Z9qYUQ)

## Publication

```bibtex
@article{lin2023airline,
  title = {{AirLine}: Efficient Learnable Line Detection with Local Edge Voting},
  author = {Lin, Xiao and Wang, Chen},
  journal = {arXiv preprint arXiv:2303.16500},
  year = {2023}
}
```
You may also [download this paper](https://arxiv.org/abs/2303.16500).
