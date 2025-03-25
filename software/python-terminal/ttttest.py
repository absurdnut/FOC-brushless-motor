import re
import serial
import math
import matplotlib.pyplot as plt
import numpy as np
import threading


class MotorSimulation:
    def __init__(self, serial_port="/dev/ttyACM0", baudrate=115200):
        """
        初始化MotorSimulation类，设置串口、图形界面等
        :param serial_port: 串口设备名称
        :param baudrate: 串口波特率
        """
        self.serial_port = serial_port
        self.baudrate = baudrate

        # 初始化图形和坐标系
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(-1.5, 1.5)
        self.ax.set_ylim(-1.5, 1.5)
        self.ax.set_aspect("equal", "box")
        self.ax.set_xticks([])  # 去掉X轴
        self.ax.set_yticks([])  # 去掉Y轴

        # 圆的参数
        self.radius = 1.0

        # 绘制圆
        self.circle = plt.Circle((0, 0), self.radius, color="gray", fill=False)
        self.ax.add_artist(self.circle)

        # 绘制指针
        (self.pointer,) = self.ax.plot([], [], "r-", lw=2)

        # 启动串口数据读取线程
        self.serial_thread = threading.Thread(target=self.read_serial_data, daemon=True)
        self.serial_thread.start()

    def read_serial_data(self):
        """
        从串口读取数据并提取Ua, Ub, Uc的数值，进行变换并绘制图形。
        """
        try:
            # 连接到串口
            ser = serial.Serial(self.serial_port, self.baudrate, timeout=1)

            while True:
                if ser.in_waiting > 0:
                    data = (
                        ser.readline().decode("utf-8").strip()
                    )  # 假设数据以换行符结束

                    # 使用正则表达式提取数字
                    match = re.match(r"Ua:(-?\d+)Ub:(-?\d+)Uc:(-?\d+)", data)

                    # 判断是否成功匹配
                    if match:
                        Ua = int(match.group(1))
                        Ub = int(match.group(2))
                        Uc = int(match.group(3))

                        # 调用Clarke变换函数
                        Ualpha, Ubeta = self.clarke_transform(Ua, Ub, Uc)

                        # 打印转换结果
                        print(f"Ua: {Ua}, Ub: {Ub}, Uc: {Uc}")
                        print(f"Ualpha: {Ualpha}, Ubeta: {Ubeta}")

                        # 计算电压幅值和角度
                        magnitude, angle = self.calculate_magnitude_and_angle(
                            Ualpha, Ubeta
                        )

                        # 绘制电机旋转轨迹
                        self.plot_motor_rotation(magnitude, angle)
        except serial.SerialException as e:
            print(f"串口连接错误: {e}")

    def clarke_transform(self, Ua, Ub, Uc):
        """
        Clarke 变换：将三相电压（Ua, Ub, Uc）转换为两相静止坐标系的电压（Ualpha, Ubeta）。
        :param Ua: 三相电压Ua
        :param Ub: 三相电压Ub
        :param Uc: 三相电压Uc
        :return: 转换后的Ualpha, Ubeta
        """
        Ualpha = Ua
        Ubeta = (1 / math.sqrt(3)) * (Ua + 2 * Ub)
        return Ualpha, Ubeta

    def calculate_magnitude_and_angle(self, Ualpha, Ubeta):
        """
        根据Clarke变换后的Ualpha和Ubeta计算电压幅值和角度。
        :param Ualpha: Clarke变换后的Ualpha
        :param Ubeta: Clarke变换后的Ubeta
        :return: 电压幅值和角度
        """
        magnitude = math.sqrt(Ualpha**2 + Ubeta**2)  # 计算幅值
        angle = math.atan2(Ubeta, Ualpha)  # 计算角度（弧度）
        return magnitude, angle

    def plot_motor_rotation(self, magnitude, angle):
        """
        绘制电机旋转的轨迹。
        :param magnitude: 电压幅值
        :param angle: 电压相位角度
        """
        # 更新指针的位置
        x = magnitude * np.cos(angle)
        y = magnitude * np.sin(angle)
        self.pointer.set_data([0, x], [0, y])
        plt.draw()

    def show(self):
        """
        显示图形界面
        """
        plt.show()

    def test_motor_rotation(self):
        """
        测试电机旋转，通过模拟数据生成电压并绘制电机旋转过程。
        """
        num_steps = 100  # 旋转100步（半圈）
        for i in range(num_steps):
            # 模拟逐渐增加角度
            angle = i * (math.pi) / num_steps  # 半圈旋转
            Ua = math.cos(angle) * 100  # 模拟电压
            Ub = math.sin(angle) * 100  # 模拟电压
            Uc = 0  # 假设Uc为0，简化计算

            # 调用Clarke变换
            Ualpha, Ubeta = self.clarke_transform(Ua, Ub, Uc)

            # 计算电压幅值和角度
            magnitude, angle = self.calculate_magnitude_and_angle(Ualpha, Ubeta)

            # 绘制电机旋转轨迹
            self.plot_motor_rotation(magnitude, angle)
            plt.pause(0.1)  # 暂停，让动画显示


if __name__ == "__main__":
    # 创建MotorSimulation实例并显示图形
    motor_sim = MotorSimulation(serial_port="/dev/ttyACM0", baudrate=115200)

    # 测试电机旋转，旋转半圈
    # motor_sim.test_motor_rotation()

    # 显示图形界面
    motor_sim.show()
