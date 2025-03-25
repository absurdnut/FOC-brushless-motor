import numpy as np
import matplotlib.pyplot as plt
import tkinter as tk

# 生成数据
x = np.linspace(-2 * np.pi, 2 * np.pi, 1000)  # x从-2π到2π
y = np.tan(x)  # 计算tan(x)

# 设置图形的y轴范围，避免tan(x)图像过于极端
y = np.clip(y, -10, 10)

# plt.switch_backend("tkagg")  # 使用非交互式后端
# 绘制图像
plt.plot(x, y, label="tan(x)", color="b")
plt.axhline(0, color="black", linewidth=1)  # 添加x轴
plt.axvline(0, color="black", linewidth=1)  # 添加y轴
plt.title("Graph of tan(x)")
plt.xlabel("x")
plt.ylabel("tan(x)")
plt.grid(True)
plt.legend()
plt.show()
