# Hovercraft25 · 气垫越野智能车

![Platform](https://img.shields.io/badge/MCU-AI8051U-blue)
![Language](https://img.shields.io/badge/Language-C-informational)
![IDE](https://img.shields.io/badge/IDE-Keil_uVision-orange)

北京交通大学威海校区 **翻斗车三队** 的气垫越野智能车控制程序，基于逐飞科技 AI8051U 开源库开发。

本项目面向自制气垫车，使用陀螺仪和编码器估计相对位置，记录并保存推车轨迹，再通过切点循迹与 PID 控制调节风扇，实现自主行驶。
此控制程序主要亮点和难点为纯惯导控制算法与四个无刷风扇的PID控制策略，均在技术报告中详细论述。

## 主要功能

- **惯性导航**：结合陀螺仪航向与编码器位移，计算平面相对坐标。
- **轨迹记录与回放**：推车采点，通过片内 Flash / IAP 保存和读取路径。
- **循迹与运动控制**：采用切点循迹思路，结合速度、航向及角速度控制，调节尾部双风扇的合力与推力差。
- **调试与交互**：支持 IPS114 屏幕显示、按键操作及 LoRa 遥控相关功能。

> 当前主程序使用 IMU660RA，系统时钟为 40 MHz，两个定时任务周期均为 5 ms。仓库保留 GPS 相关代码，但主程序中的 GPS 初始化与 GPS 采点调用已注释，当前默认采用陀螺仪与编码器导航。

## 目录结构

| 路径 | 内容 |
| --- | --- |
| [project/user/main.c](project/user/main.c) | 程序入口、初始化、任务调度与轨迹存取 |
| [project/user/isr.c](project/user/isr.c) | 中断服务 |
| [project/code/](project/code/) | 编码器、陀螺仪、角度、遥控、GPS 等应用模块 |
| [project/mdk/](project/mdk/) | Keil 工程及控制相关源文件 |
| [libraries/](libraries/) | 逐飞公共组件、外设驱动与设备库 |

## 开发与使用

1. 克隆仓库：
   ```bash
   git clone https://github.com/mrnobodynan/Hovercraft25.git
   ```
2. 使用 Keil uVision 打开 [project/mdk/seekfree.uvproj](project/mdk/seekfree.uvproj)，配置匹配 AI8051U / STC8051U-32Bit 的工具链与设备支持。
3. 按实际车模核对 IMU、编码器、屏幕、LoRa 与电调接线，以及 PWM 输出配置，再编译并通过适配的 STC 下载工具烧录。
4. 完成陀螺仪零偏与编码器参数校准，检查风扇方向和输出范围，再进行轨迹采集、Flash 保存及循迹调参。

控制参数、引脚与轨迹数据依赖实际车模，移植时需要重新核对。本文仅更新项目说明，未进行硬件编译或实车验证。

## 项目来源与说明

- 项目背景与控制方案依据本地技术报告《北京交通大学威海校区_翻斗车三队_气垫越野》整理；报告已随仓库发布。
- 基础库来源于 **逐飞科技（SEEKFREE）AI8051U 开源库**，原有版权与许可声明以对应源码为准。
- 技术报告指出，纯惯性导航仍存在随行驶距离和转弯累积的误差；GPS 融合属于后续改进方向。

**项目标签**：智能车竞赛 · 气垫越野 · AI8051U · 嵌入式 C · 惯性导航 · PID · 路径跟踪
