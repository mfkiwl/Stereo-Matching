# -*- coding: utf-8 -*-
"""
Created on Thu Mar 18 09:08:18 2021

@author: ASUS
"""


import numpy as np

import math

# Checks if a matrix is a valid rotation matrix.
def isRotationMatrix(R) :
    Rt = np.transpose(R)
    shouldBeIdentity = np.dot(Rt, R)
    I = np.identity(3, dtype = R.dtype)
    n = np.linalg.norm(I - shouldBeIdentity)
    return n < 1e-6
 
 
# Calculates rotation matrix to euler angles
# The result is the same as MATLAB except the order
# of the euler angles ( x and z are swapped ).
def rotationMatrixToEulerAngles(R) :
 
    assert(isRotationMatrix(R))
     
    sy = math.sqrt(R[0,0] * R[0,0] +  R[1,0] * R[1,0])
     
    singular = sy < 1e-6
 
    if  not singular :
        x = math.atan2(R[2,1] , R[2,2])
        y = math.atan2(-R[2,0], sy)
        z = math.atan2(R[1,0], R[0,0])
    else :
        x = math.atan2(-R[1,2], R[1,1])
        y = math.atan2(-R[2,0], sy)
        z = 0
 
    return np.array([x, y, z])


R=np.zeros((3,3))


# R[0, 0] = 0.999960931998933;
# R[0, 1]  = 0.001511786658302;
# R[0, 2]  = -0.008709131812311;
# R[1, 0]  = -0.001483010750471;
# R[1, 1]  = 0.999993423530581;
# R[1, 2] = 0.003309618512966;
# R[2, 0]  = 0.008714077974084;
# R[2, 1] = -0.003296573476681;
# R[2, 2] = 0.999956597782311;


R[0, 0]  = 0.999777073616913;
R[0, 1] = 8.276943022604731e-04;
R[0, 2]= -0.021097819606417;
R[1, 0] = -7.787013368596594e-04;
R[1, 1] = 0.999996981679368;
R[1, 2] = 0.002330291051101;
R[2, 0] = 0.021099684695058;
R[2, 1]= -0.002313342667413;
R[2, 2]  = 0.999774700495803;
    
eular_radian=rotationMatrixToEulerAngles(R)

eular_angle=eular_radian*180/np.pi