# -*- coding: utf-8 -*-
"""
Created on Mon Nov  9 14:44:53 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title：script-ransac example
"""

import numpy as np
import matplotlib.pyplot as plt
import random

import calculation_ransac as C_R

# 数据量。
SIZE = 50
# 产生数据。np.linspace 返回一个一维数组，SIZE指定数组长度。
# 数组最小值是0，最大值是10。所有元素间隔相等。
X = np.linspace(0, 10, SIZE)
Y = 3 * X + 10

fig = plt.figure()
# 画图区域分成1行1列。选择第一块区域。
ax1 = fig.add_subplot(1,1, 1)
# 标题
ax1.set_title("RANSAC")

# 让散点图的数据更加随机并且添加一些噪声。
random_x = []
random_y = []
# 添加直线随机噪声
for i in range(SIZE):
    random_x.append(X[i] + random.uniform(-0.5, 0.5)) 
    random_y.append(Y[i] + random.uniform(-0.5, 0.5)) 
# 添加随机噪声
for i in range(SIZE):
    random_x.append(random.uniform(0,10))
    random_y.append(random.uniform(10,40))
RANDOM_X = np.array(random_x) # 散点图的横轴。
RANDOM_Y = np.array(random_y) # 散点图的纵轴。

# 画散点图。
ax1.scatter(RANDOM_X, RANDOM_Y)
# 横轴名称。
ax1.set_xlabel("x")
# 纵轴名称。
ax1.set_ylabel("y")

C_R.RANSAC(RANDOM_X,RANDOM_Y)