# Capacitated Facility Location Problem

## [Problem Description](https://en.wikipedia.org/wiki/Facility_location_problem)

Suppose there are n facilities and m customers. We wish to choose:

(1) which of the n facilities to open

(2) the assignment of customers to facilities

(3) The objective is to minimize the sum of the opening cost and the assignment cost.

(4) The total demand assigned to a facility must not exceed its capacity.

## [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm)

遗传算法是受遗传学中自然选择和遗传机制启发发展起来的一种搜索算法。基本思想是使用模拟生物和人类进化的方法求解复杂的优化问题，因而也称作为模拟进化优化算法。模拟进化优化算法在计算原理上是自适应的，结构上是并行的，而且模仿了人的智能处理特征，因而称为人工智能的一个重要研究领域。

#### 基本定义

**个体（individual）：**个体是一个数据结构，用来描述基本的遗传结构。例如，用0、1组成的串可以表示个体。这样的串叫染色体，其中每个0或1叫等位基因。这样的一个串于某个个体相关联，则称为该个体的基因型。

**适应性（fitness）：**每个个体有一对应的适应值。再优化问题中，适应值来自于一个估计函数。

**群体（population）：**有个体组成的集合。

