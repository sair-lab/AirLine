---
layout: post
title: "Lifelong Graph Learning"
date: 2022-3-5 12:00:00
categories: research
description: "Lifelong Graph Learning"
author: "Chen Wang"
published: true
sidebar:  false
permalink: /lgl/
image: /img/posts/2022-03-05-lgl/matching.jpg
datatable: true
hero_height: is-small
title_image: None
link-new-tab: true
---

Graph neural networks (GNNs) are powerful models for many graph-structured tasks. Existing models often assume that a complete structure of a graph is available during training. However, graph-structured data is often formed in a streaming fashion so that learning a graph continuously is often necessary.

<figure>
    <img src="/img/image (1).png" />
    <figcaption>
        A temporally growing graph, which is challenging to learn the graph in a sequential way.
    </figcaption>
</figure>

Existing lifelong learning techniques are mostly designed for convolutional neural networks (CNNs), which *assumes the new data samples are independent*. However, in lifelong graph learning, *nodes are connected and dynamically added*.
In this work, we have an important observation:

* **The number of nodes increases dynamically, while the number of node features is stable.**

Therefore, we introduce feature graph, which **takes features as nodes and turns nodes into independent graphs**. This successfully converts the original problem of node classification to graph classification, in which the increasing nodes are turned into independent training samples.

<figure>
    <img src="/img/posts/2022-03-05-lgl/feature-graph.jpg"/>
    <figcaption>
        Feature graph takes the features as nodes and turns nodes into graphs, resulting in a graph predictor instead of the node predictor. Take the node a with label <img src="https://render.githubusercontent.com/render/math?math=\mathbf{z}_a"> in the regular graph <img src="https://render.githubusercontent.com/render/math?math=\mathcal{G}"> as an example, its features <img src="https://render.githubusercontent.com/render/math?math=x_a = [1, 0, 0, 1]">  are nodes <img src="https://render.githubusercontent.com/render/math?math=\{a1, a2, a3, a4\}"> in feature graph <img src="https://render.githubusercontent.com/render/math?math=\mathcal{G}^F">. 
    </figcaption>
</figure>

The feature adjacency is established via feature cross-correlation between <img src="https://render.githubusercontent.com/render/math?math=a"> and its neighbors <img src="https://render.githubusercontent.com/render/math?math=\mathcal{N}(a) = \{a, b, c, d, e\}"> to model feature “interaction.”  This makes the lifelong learning techniques for CNN applicable to GNN, as the new nodes in a regular graph become individual training samples.

<figure>
    <figcaption>
        The relationship of a graph and feature graph.
    </figcaption>
    <img src="/img/posts/2022-03-05-lgl/relationship.jpg"/>
</figure>

## Applications

#### 1. Feature Matching

Image feature matching is crucial for many 3-D computer vision tasks including simultaneous localization and mapping (SLAM).
As shown below, the interest point and their descriptors form an infinite temporal growing graph, in which the feature points are nodes and their descriptors are the node features. In this way, the problem of feature matching becomes edge prediction for a temporal growing graph. 

<figure>
    <img src="/img/posts/2022-03-05-lgl/matching.jpg"/>
    <figcaption>
         Feature matching is a problem of edge prediction for temporal growing graph.
    </figcaption>
</figure>

In an environment with dynamic illumination, feature graph network can also achieve much better accuracy and robustness, which demonstrate its effectiveness.

<figure>
    <img src="/img/posts/2022-03-05-lgl/matching.gif"/>
    <figcaption>
        Feature fatching in an environment with dynamic illumination.
    </figcaption>
</figure>

#### 2. Distributed Human Action Recognition with Wearable Devices

Five sensors, each of which consists of a triaxial accelerometer and a biaxial gyroscope, are located at the left and right forearms, waist, left and right ankles, respectively. Each sensor produces 5 data streams and totally 5 × 5 data streams is available. 13 daily action categories are considered, including rest at standing (ReSt), rest at sitting (ReSi), rest at lying (ReLi), walk forward (WaFo), walk forward left-circle (WaLe), walk forward right-circle (WaRi), turn left (TuLe), turn right (TuRi), go upstairs (Up), go downstairs (Down), jog (Jog), jump (Jump), and push wheelchair (Push). Therefore, action recognition is a problem of sub-graph classification.

<figure>
    <img src="/img/posts/2022-03-05-lgl/ward.jpg"/>
    <figcaption>
        Lifelong human action recognition is a problem of lifelong sub-graph classification.
    </figcaption>
</figure>

Our feature graph network (FGN) has a much higher and stable performance than all the other
methods, including GCN, APPNP, and GAT. It also achieves a much higher final per-class precision in nearly all the categories.

* **FGN is the first method to bridge graph learning to lifelong learning via a novel graph topology.**

## Video

{% youtube 711oo3Mi2Do %}

## Source Code

* **[Lifelong Graph Learning (Citation Graph)](https://github.com/wang-chen/LGL)**

* **[Lifelong Graph Learning (Action Recognition)](https://github.com/wang-chen/lgl-action-recognition)**

* **[Lifelong Graph Learning (Feature Matching)](https://github.com/wang-chen/lgl-feature-matching)**

## Publications

{% bibliography --query @*[title=Lifelong Graph Learning] %}
