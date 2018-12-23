# Capacitated Facility Location Problem

## [Problem Description](https://en.wikipedia.org/wiki/Facility_location_problem)

Suppose there are n facilities and m customers. We wish to choose:

(1) which of the n facilities to open

(2) the assignment of customers to facilities

(3) The objective is to minimize the sum of the opening cost and the assignment cost.

(4) The total demand assigned to a facility must not exceed its capacity.

## [遗传算法（genetic algorithm）](https://en.wikipedia.org/wiki/Genetic_algorithm)

遗传算法是受遗传学中自然选择和遗传机制启发发展起来的一种搜索算法。基本思想是使用模拟生物和人类进化的方法求解复杂的优化问题，因而也称作为模拟进化优化算法。模拟进化优化算法在计算原理上是自适应的，结构上是并行的，而且模仿了人的智能处理特征，因而称为人工智能的一个重要研究领域。

### 编译执行
```bash
GA$ g++ main.cpp ./customer/customer.cpp ./facility/facility.cpp -std=c++11
GA$ ./a.exe
```
运行结果查看Result/GA.svc

## [模拟退火（Simulated Annealing）](https://zh.wikipedia.org/wiki/%E6%A8%A1%E6%8B%9F%E9%80%80%E7%81%AB)

模拟退火是克服爬山缺点的有效方案，其基本思想是在系统能量减小的趋势这样一个变化过程中，偶尔允许系统跳到能量较高的状态，以避开局部极小点，最终稳定到全局最小点。

### 编译执行
```bash
GA$ g++ main.cpp ./customer/customer.cpp ./facility/facility.cpp -std=c++11
GA$ ./a.exe
```
运行结果查看Result/SA.svc

## [博客链接](https://blog.csdn.net/liuyh73/article/details/85223634)
