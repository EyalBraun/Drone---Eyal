## Kalman Filter - System Architecture Summary

**Goal:** To provide a high-precision estimate of the drone's orientation ($\theta$) and the gyroscope's bias by fusing noisy sensor data and accounting for external disturbances like wind and air density changes.

---

### 1. The Core Concept: Sensor Fusion

The filter merges two distinct data sources, dynamically weighting them based on their reliability:

- **Gyroscope (The Predictor):** High-speed response, ideal for tracking the fast angular changes mentioned in your PID logic. **Weakness:** Subject to "drift" (accumulating error over time).
    
- **Accelerometer (The Observer):** Provides a long-term stable reference relative to gravity. **Weakness:** Highly susceptible to "noise" from motor vibrations and external accelerations.
    

---

### 2. The Functional Loop

The filter runs at **400Hz** to match your control loop frequency, following these stages:

1. **Prediction:** Using the **Perfect Last Estimate** ($\hat{x}_{k-1}$) and the current gyroscope reading ($\omega$) to "guess" the new angle.
    
2. **Innovation (Error Check):** Calculating the difference between the "guessed" angle and the angle measured by the accelerometer.
    
3. **Correction (Kalman Gain $K$):** This is the "brain" of the filter. It decides who to believe:
    
    - **High Dynamics:** If the drone is rotating fast, $K$ tilts toward the **Gyroscope**.
        
    - **Static/Hover:** In **Hover Mode** ($v=0$), $K$ tilts toward the **Accelerometer** to eliminate gyro drift.
        

---

### 3. Noise Management ($Q$ vs $R$)

- **Process Noise ($Q$):** Represents uncertainty in the model (e.g., turbulence, wind gusts, or changing air density).
    
- **Measurement Noise ($R$):** Represents sensor noise. In your design, $R$ increases during high **Differential Thrust** ($C$) due to increased mechanical vibration.
    

---

### 4. Mathematical State Vector ($\hat{x}$)

We do not track the "raw" angle, but the **Estimated "Perfect" State**:

$$\hat{x} = \begin{bmatrix} \text{Estimated Angle } (\theta) \\ \text{Estimated Gyro Bias } (\dot{\theta}_{bias}) \end{bmatrix}$$

- **$\theta$:** The fused, clean angle used by the PID `Error` calculation.
    
- **$\dot{\theta}_{bias}$:** The learned offset of the gyroscope. The filter constantly calculates this to subtract it from the raw gyro data, preventing long-term drift.