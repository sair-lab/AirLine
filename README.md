---
layout: post
hero_height: is-small
title: "TartanAir: A Dataset to Push the Limits of Visual SLAM"
date: 2020-02-29 13:55:07
categories: datasets
description: "TartanAir"
author: "Wenshan Wang"
published: true
sidebar: false
permalink: /tartanair-dataset/
image: /img/posts/2020-02-29-tartanair/web_cover_figure.png
datatable: true
link-new-tab: true
---

## Abstract ## 

We present a challenging dataset, the TartanAir, for robot navigation task and more. The data is collected in photo-realistic simulation environments in the presence of various light conditions, weather and moving objects. By collecting data in simulation, we are able to obtain multi-modal sensor data and precise ground truth labels, including the stereo RGB image, depth image, segmentation, optical flow, camera poses, and LiDAR point cloud. We set up a large number of environments with various styles and scenes, covering challenging viewpoints and diverse motion patterns, which are difficult to achieve by using physical data collection platforms. In order to enable data collection in such large scale, we develop an automatic pipeline, including mapping, trajectory sampling, data processing, and data verification. We evaluate the impact of various factors on visual SLAM algorithms using our data. Results of state-of-the-art algorithms reveal that the visual SLAM problem is far from solved, methods that show good performance on established datasets such as KITTI don't perform well in more difficult scenarios. Although we use the simulation, our goal is to push the limits of Visual SLAM algorithms in the real world by providing a challenging benchmark for testing new methods, as well as large diverse training data for learning-based methods. 


## Download

The dataset is published using Azure Open Dataset platform. Please checkout [here](https://github.com/castacks/tartanair_tools) for the instruction of accessing the data.  

Sample trajectories can be downloaded here. 

| <img src="/img/posts/2020-02-29-tartanair/abandonedfactory.gif" /> <br/> [abandonedfactory][abandonedfactory_sample] | <img src="/img/posts/2020-02-29-tartanair/abandonedfactory_night.gif" /> <br/> [abandonedfactory_night][abandonedfactory_night_sample] | <img src="/img/posts/2020-02-29-tartanair/amusement.gif" /> <br/> [amusement][amusement_sample] | <img src="/img/posts/2020-02-29-tartanair/carwelding.gif" /> <br/> [carwelding][carwelding_sample] | 
| <img src="/img/posts/2020-02-29-tartanair/endofworld.gif" /> <br/> [endofworld][endofworld_sample] | <img src="/img/posts/2020-02-29-tartanair/gascola.gif" /> <br/> [gascola][gascola_sample] |  <img src="/img/posts/2020-02-29-tartanair/hospital.gif" /> <br/> [hospital][hospital_sample] | <img src="/img/posts/2020-02-29-tartanair/jananesealley.gif" /> <br/> [japanesealley][japanesealley_sample] |
| <img src="/img/posts/2020-02-29-tartanair/neighborhood.gif" /> <br/> [neighborhood][neighborhood_sample] | <img src="/img/posts/2020-02-29-tartanair/ocean.gif" /> <br/> [ocean][ocean_sample] | <img src="/img/posts/2020-02-29-tartanair/office.gif" /> <br/> [office][office_sample] |  <img src="/img/posts/2020-02-29-tartanair/seasidetown.gif" /> <br/> [seasidetown][seasidetown_sample] |
| <img src="/img/posts/2020-02-29-tartanair/seasonsforest.gif" /> <br/> [seasonsforest][seasonsforest_sample] | <img src="/img/posts/2020-02-29-tartanair/seasonsforest_winter.gif" /> <br/> [seasonsforest_winter][seasonsforest_winter_sample] | <img src="/img/posts/2020-02-29-tartanair/soulcity.gif" /> <br/> [soulcity][soulcity_sample] | <img src="/img/posts/2020-02-29-tartanair/westerndesert.gif" /> <br/> [westerndesert][westerndesert_sample] |

<!-- <img src="/img/posts/2020-02-29-tartanair/hongkongalley.gif" /> <br/> [hongkongalley](http://dummy) --> 
<!-- <img src="/img/posts/2020-02-29-tartanair/house.gif" /> <br/> [house](http://dummy)  -->
<!-- <img src="/img/posts/2020-02-29-tartanair/oldtown.gif" /> <br/> [oldtown](http://dummy)  -->
<!-- <img src="/img/posts/2020-02-29-tartanair/slaughter.gif" /> <br/> [slaughter](http://dummy)  -->

[abandonedfactory_sample]: https://gatech.app.box.com/file/1176551556704
[abandonedfactory_night_sample]: https://cmu.box.com/s/zohqnu8mglwh4hw2zszxoqt69hc1pwha
[amusement_sample]: https://cmu.box.com/s/mn7z8dwr93wl23zngwi3q76kyq4dbkv7
[carwelding_sample]: https://cmu.box.com/s/qpoikn7owhhj2v718m8u9cdmpsqmuq14
[endofworld_sample]: https://cmu.box.com/s/jk2pihbq94eicvd0vr7hjme26bn1u2tt
[gascola_sample]: https://cmu.box.com/s/fxtytvvbbn0e2g9flhr2br1eiibj8dfs
[hospital_sample]: https://cmu.box.com/s/clj21v7ancdhxe6u87tp56b638e4rur0
[japanesealley_sample]: https://cmu.box.com/s/19aj7mob1s4912tmg36lotuiwgf0x5fh
[neighborhood_sample]: https://cmu.box.com/s/5trtb7f3ogjao33lgk6xu6t9y9nu79wg
[ocean_sample]: https://cmu.box.com/s/1egqcvrfqzg84ctpu81x2ww00y1xcpfv
[office_sample]: https://cmu.box.com/s/nem5fglri6fbfa0t5rm854l67q0fu0l9
[seasidetown_sample]: https://cmu.box.com/s/zzwyrrqm2ir2z0z75tqowpq91gny2sjk
[seasonsforest_sample]: https://cmu.box.com/s/nssib68sq3ilp0qy0r3zl347wtkophgb
[seasonsforest_winter_sample]: https://cmu.box.com/s/lwrzi0d338857qy79odarmsgtbp5j0qg
[soulcity_sample]: https://cmu.box.com/s/cusfpmiskrgn8l3h0t1hivwt81mcfdk9
[westerndesert_sample]: https://cmu.box.com/s/0dz8ybjgontw59k3g2s2l43b80hyk1u1

## The mission ##

Simultaneous Localization and Mapping (SLAM) is one of the most fundamental capabilities necessary for robots. Due to the ubiquitous availability of images, Visual SLAM (V-SLAM) has become an important component of many autonomous systems. Impressive progress has been made with both geometric-based methods and learning-based methods. However, developing robust and reliable SLAM methods for real-world applications is still a challenging problem. Real-life environments are full of difficult cases such as light changes or lack of illumination, dynamic objects, and texture-less scenes. Current popular benchmarks such as KITTI, TUM RGB-D SLAM datasets, and EuRoC MAV cover relatively limited scenarios and motion patterns compared to real-world cases. 

We collect a large dataset using photo-realistic simulation environments. We minimize the sim2real gap by utilizing a large number of environments with various styles and diverse scenes. A special goal of our dataset is to focus on the challenging environments with changing light conditions, adverse weather, and dynamic objects. State-of-the-art SLAM algorithms are struggled in tracking the camera pose in our dataset and constantly getting lost on some challenging sequences. We propose a metric to evaluate the robustness of the algorithm. In addition, we develop an automatic data collection pipeline, which allows us to process more environments with minimum human intervention. 

The four most important features of our dataset are: 

- Large size diverse realistic data
- Multimodal ground truth labels
- Diversity of motion patterns
- Challenging Scenes

<!-- <span style="color:red"> A youtube video should go here. </span> -->


## Dataset features ##

### Simulated scenes ###
We have adopted 30 photo-realistic simulation environments in the [Unreal Engine][UnrealEngine]. The environments provide us a wide range of scenarios that cover many interesting yet challenging situations. The simulation scenes consist of

[UnrealEngine]: https://www.unrealengine.com/

- Indoor and outdoor scenes with detailed 3D objects. We have multi-room, richly decorated indoor environments. For outdoor simulations, there are various kinds of buildings, trees, terrains, and landscapes.
- Special purpose facilities and ordinary household scenes.
- Rural and urban scenes.
- Real-life and sci-fi scenes.

<figure>
 <img src="/img/posts/2020-02-29-tartanair/environments.png" alt="Simulated environments" />
 <figcaption>
 A glance of the simulated environments.
 </figcaption>
</figure>

### Challenging visual effects ###
In some simulations, we simulated multiple types of challenging visual effects.

- Hard lighting conditions. Day-night alternating. Low-lighting. Rapidly changing illuminations.
- Weather effects. Clear, raining, snowing, windy, and fog.
- Seasonal change.

<figure>
 <img src="/img/posts/2020-02-29-tartanair/visual_effects.png" alt="The challenging visual effects." />
 <figcaption>
 Challenging visual effects.
 </figcaption>
</figure>

### Diverse ego motions ###

In each simulated environment, we gather data by following multiple routes and making movements with different levels of aggressiveness. The virtual camera can move slowly and smoothly without sudden jittering actions. Or it can have intensive and violent actions mixed with significant rolling and yaw motions.


### Multimodal ground truth labels ###

By unleashing the power of the [Unreal Engine][UnrealEngine] and [AirSim][AirSimSite], we can extract various types of ground truth labels including depth, semantic segmentation tag, and camera pose. From the extracted raw data, we further compute other ground truth labels such as optical flow, stereo disparity, simulated multi-line LiDAR points, and simulated IMU readings.

[AirSimSite]: https://github.com/microsoft/AirSim

<figure>
 <img src="/img/posts/2020-02-29-tartanair/multimodal_data_20200301.png" alt="Data and ground truth labels." />
 <figcaption>
 Data and ground truth labels.
 </figcaption>
</figure>

## Data acquisition pipeline ##

We develop a highly automated pipe-line to facilitate data acquisition. For each environment, we build an occupancy map by incremental mapping. Base on the map, we then sample a bunch of trajectories for the virtual camera to follow. A set of virtual cameras follow the trajectories to capture raw data from Unreal Engine and AirSim. Raw data are processed to generate labels such as optical flow, stereo disparity, simulated LiDAR points, and simualated IMU readings. Data verification. Verify the data synchronization and the accuracy of the derived labels.

<figure>
 <img src="/img/posts/2020-02-29-tartanair/pipeline_20200301.png" alt="Data acquisition pipeline." />
 <figcaption>
 Data acquisition pipeline.
 </figcaption>
</figure>

{% youtube zJ8u_ACEP9c %}

### Publications

{% bibliography --query @*[title=TartanAir: A Dataset to Push the Limits of Visual SLAM] %}


### Contact

Email tartanair@hotmail.com if you have any questions about the data source. You can also reach out to contributors on the associated [GitHub](https://github.com/microsoft/AirSim).

Wenshan Wang - (wenshanw [at] andrew [dot] cmu [dot] edu) 

Sebastian Scherer - (basti [at] cmu [dot] edu)

### Acknowledgments 

This work was supported by Office of Naval Research under award number N0014-19-1-2266. We thank Ratnesh Madaan, Guada Casuso, Rashaud Savage and other team members from Microsoft for the technical support! 

### Term of use

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.