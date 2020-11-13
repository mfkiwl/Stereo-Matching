# -*- coding: utf-8 -*-
"""
Created on Mon Nov  9 18:05:39 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：Module-RANSAC
"""

import matplotlib.pyplot as plt
import random
import math

def RANSAC(RANDOM_X,RANDOM_Y):
    
    SIZE=len(RANDOM_X)
    
    # 使用RANSAC算法估算模型
    # 迭代最大次数，每次得到更好的估计会优化iters的数值
    iters =6666
    # 数据和模型之间可接受的差值
    sigma = 0.25
    # 最好模型的参数估计和内点数目
    best_a = 0
    best_b = 0
    pretotal = 0
    # 希望的得到正确模型的概率
    P = 0.99
    for i in range(iters):
        
        # 随机在数据中红选出两个点去求解模型
        sample_index = random.sample(range(SIZE),2)
        x_1 = RANDOM_X[sample_index[0]]
        x_2 = RANDOM_X[sample_index[1]]
        y_1 = RANDOM_Y[sample_index[0]]
        y_2 = RANDOM_Y[sample_index[1]]
    
        # y = ax + b 求解出a，b
        a = (y_2 - y_1) / (x_2 - x_1)
        b = y_1 - a * x_1
    
        # 算出内点数目
        total_inlier = 0
        for index in range(SIZE):
            y_estimate = a * RANDOM_X[index] + b
            if abs(y_estimate - RANDOM_Y[index]) < sigma:
                total_inlier = total_inlier + 1
    
        # 判断当前的模型是否比之前估算的模型好
        if total_inlier > pretotal:
            
            # iters = math.log(1 - P) / math.log(1 - pow(total_inlier / (SIZE), 2))
            pretotal = total_inlier
            best_a = a
            best_b = b
    
        # 判断是否当前模型已经符合超过一半的点
        if total_inlier > SIZE:
            break
    
    # 用我们得到的最佳估计画图
    Y = best_a * RANDOM_X + best_b
    
    ax=plt.gca()
    
    # 直线图
    ax.plot(RANDOM_X, Y)
    text = "best_a = " + str(best_a) + "\nbest_b = " + str(best_b)
    plt.text(5,10, text,
             fontdict={'size': 8, 'color': 'r'})
    plt.show()
    
    return Y