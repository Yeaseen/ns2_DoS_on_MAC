#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr 21 19:47:51 2019

@author: yeaseen
"""


from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split
import numpy as np

def split(s, delim):
    words = []
    word = []
    for c in s:
        if c not in delim:
            word.append(c)
        else:
            if word:
                words.append(''.join(word))
                word = []
    if word:
        words.append(''.join(word))
    return words

def loadfile(filename):
    file = open(filename, "r")
    rows = list()
    for line in file:
        vals = split(line, [' ' ,'\t', '\n'])
        rows.append(vals)
    return rows



'''
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')


for row in data:
    xs=row[0]
    ys=row[1]
    zs=row[2]
    
    if(row[3]==0):
        c='r'
        m='o'
    else:
        c='b'
        m='^'
    ax.scatter(xs, ys, zs, c=c, marker=m)

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()
'''





#

data=loadfile('sum.txt')
data=np.array(data)
data= data.astype(np.float)

dataY=data[:,-1].copy()
dataY=np.array(dataY)

dataX=np.delete(data,-1,axis=1)

X_train, X_test, Y_train, Y_test = train_test_split(dataX, dataY, test_size=0.2, random_state=42)


clf = LinearSVC(random_state=0, tol=1e-5)

print("started fitting")
clf.fit(X_train, Y_train)

print("w:")
w=clf.coef_
print(w)
print("b:")
b=clf.intercept_
print(b)

PY_train=[]
for r in X_train:
    value=clf.predict([[r[0], r[1], r[2]]])
    PY_train.append(value[0])

from sklearn.metrics import accuracy_score
print("Accuracy is in train set:   "+str(accuracy_score(Y_train, PY_train)))


ww=np.array([2.59804693, 2.52596639, -0.36001139, -1.09308601])
PY_test=[]
PYY_test=[]
for r in X_test:
    #print(clf.predict([[r[0], r[1], r[2]]]))
    #value=clf.predict([[r[0], r[1], r[2]]])
    #PY_test.append(value[0])
    rr=np.array([r[0], r[1], r[2], 1.0])
    val=np.matmul(ww,rr.T)
    if(val<0):
        PYY_test.append(0.0)
    elif(val>=0):
        PYY_test.append(1.0)
    

from sklearn.metrics import accuracy_score
print("Accuracy is test set:   "+str(accuracy_score(Y_test, PYY_test)))


index=[]
for i in range(len(PYY_test)): 
    if Y_test[i]!=PYY_test[i]:
        index.append(np.insert(X_test[i],3,Y_test[i]))
       





















