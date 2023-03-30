

# Abstract

Line detection is widely used in many robotic tasks such as scene recognition, 3D reconstruction, and simultaneous localization and mapping (SLAM). Compared to points, lines can provide both low-level and high-level geometrical information for downstream tasks. In this paper, we propose a novel edge-based line detection algorithm, AirLine, which can be applied to various tasks. In contrast to existing learnable endpoint-based methods which are sensitive to the geometrical condition of environments, AirLine can extract line segments directly from edges, resulting in a better generalization ability for unseen environments. Also to balance efficiency and accuracy, we introduce a region-grow algorithm and local edge voting scheme for line parameterization. To the best of our knowledge, AirLine is one of the first learnable edge-based line detection methods. Our extensive experiments show that it retains state-of-the-art-level precision yet with a $3-80\times$ runtime acceleration compared to other learning-based methods, which is critical for low-power robots.
<div class="center">
    <video width="49%" autoplay loop controls muted>
        <source src="/img/demo.mp4" type="video/mp4">
    </video>
</div>

<figure>
    <img src="/img/cp2.png" />
    <figcaption>
        A qualitative comparison across difference methods on untrained images.
    </figcaption>
    <img src="/img/vsLSD.gif" />
    <figcaption>
        this gif showcases thenstability and great generalization ability of AirLine compared to LSD.
    </figcaption>
    <img src="/img/qualitativecomp.gif" />
    <figcaption>
        A qualitative comparison across difference methods on untrained sequence from UMA_VI.
    </figcaption>
    
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

## Video

* **[Youtube](https://www.youtube.com/watch?v=EKDx3Z9qYUQ)**

## Source Code

* **[AirLine](https://github.com/sair-lab/AirLine)**

## Publications
