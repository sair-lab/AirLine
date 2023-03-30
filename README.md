

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
By using edge-based learning line detection methods, AirLine has achieved state-of-art-level precision and significant efficiency and runtime boost.
<figure>
    <img src="/img/wffig.png" width=48%><img src="/img/yufig.png" width=48%/>
</figure>


## Video

* **[Youtube](https://www.youtube.com/watch?v=EKDx3Z9qYUQ)**

## Source Code

* **[AirLine](https://github.com/sair-lab/AirLine)**

## Publications
