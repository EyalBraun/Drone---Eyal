Custom Flight Control System | C++ & Real-Time Control
A high-performance flight controller developed from the ground up for multi-rotor UAVs. This project focuses on the implementation of advanced control algorithms to achieve stable flight through precise sensor fusion and high-frequency feedback loops.

🧠 Core Engineering Features
Custom PID Controller: A hand-coded Proportional-Integral-Derivative (PID) algorithm for Pitch, Roll, and Yaw stabilization. Includes anti-windup logic and derivative filtering to handle high-frequency mechanical noise.

Sensor Fusion via Kalman Filter: Implementation of a linear Kalman Filter to fuse Accelerometer and Gyroscope data, providing a clean and lag-free estimation of the drone's attitude (orientation).

High-Frequency Loop: The control loop is optimized to run at 400Hz, ensuring minimal latency between sensor acquisition and motor PWM updates.

I2C/SPI Communication: Low-level drivers for interfacing with MPU-6050/9250 IMUs and barometric sensors.
