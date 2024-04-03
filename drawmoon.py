import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import make_circles
from sklearn.datasets import make_moons

def draw_circle_and_moon():
    # 生成圆形数据集
    X_circle, Y_circle = make_moons(n_samples=100,noise=0.2)

    # 将数据写入文件
    with open("moon.data", "w") as f:
        for x, y in zip(X_circle, Y_circle):
            f.write('%f %f %d\n' %(x[0],x[1],y))

draw_circle_and_moon()
