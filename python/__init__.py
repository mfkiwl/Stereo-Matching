# -*- coding: utf-8 -*-
"""
Created on Mon Nov 23 20:18:27 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: initialization script
"""

import sys,os
    
sys.path.append(os.getcwd())
sys.path.append(os.getcwd()+'\\Module')
sys.path.append(os.getcwd()+'\\Object')
sys.path=list(set(sys.path))

import cv2
import numpy as np
import matplotlib.pyplot as plt

from configuration_font import legend_prop,\
                               text_prop,\
                               label_prop,\
                               title_prop,\
                               annotation_prop
                               
import calculation_ransac as C_R

import calculation_feature_matching as C_F_M