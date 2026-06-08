# 🚦 IoT-Based Movable Road Divider with AI Traffic Management

<div align="center">

![Project Banner](https://img.shields.io/badge/NIT%20Jamshedpur-B.Tech%20Project-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)
![Language](https://img.shields.io/badge/Languages-Python%20|%20C++-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

**A smart infrastructure system that dynamically adjusts road lane configurations using IoT, Computer Vision, and real-time traffic analysis.**



</div>

---

##  Overview

Traditional road dividers are **static** — they can't respond to changing traffic conditions, causing unnecessary congestion and accidents. This project solves that by building a **smart, motorized road divider** that:


-  Uses a **Raspberry Pi + camera** to detect real-time vehicle density via **YOLOv5 TFLite** object detection
-  Runs **Non-Maximum Suppression (NMS)** and confidence filtering to accurately count vehicles per lane
-  Uses an **ESP8266 Wi-Fi module** to receive remote control commands via a web server
-  Drives physical divider movement using an **L298N motor driver**
-  Supports **remote control via mobile app** (any browser-based controller over Wi-Fi AP)

>  This project was built as a B.Tech Final Year Project at **NIT Jamshedpur**, Department of Electronics & Communication Engineering, under the supervision of **Dr. Basudeb Behera**.

---

##  Key Features

| Feature | Description |
|---|---|
|  **YOLOv5 Vehicle Detection** | Real-time vehicle detection using a quantized TFLite model on Raspberry Pi |
|  **Custom NMS in NumPy** | Implemented Non-Maximum Suppression from scratch without OpenCV/torchvision |
|  **ESP8266 Web Server** | Hosts a lightweight HTTP server for remote directional + speed control |
|  **PWM Speed Control** | 10-level speed adjustment (400–1023 PWM) via motor driver |
|  **PiCamera2 Integration** | Live image capture pipeline with proper resource management |
|  **Multi-directional Movement** | Forward, backward, left, right, and all diagonal directions supported |

---

##  System Architecture




