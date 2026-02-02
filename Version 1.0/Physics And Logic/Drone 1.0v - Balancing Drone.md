## 1. The PID Formula (Differential Thrust)

To maintain stability (Balance), we calculate a **Correction** value based on orientation error.

$$Correction = (K_p \cdot Error) - (K_d \cdot \frac{\Delta Angle}{\Delta t})$$

- **Error:** `Target_Angle (0°) - Current_Angle`.
    
- **$K_p$ (Proportional):** The "Muscle". Converts degrees of error into Newtons of force.
    
- **$K_d$ (Derivative):** The "Brake". Uses angular velocity to prevent overshoot and oscillations.
    
- **$\Delta t$:** Time elapsed since the last frame (measured in microseconds).
    

## 2. Stability Logic

- **Goal:** Level flight ($0^{\circ}$ on Pitch and Roll).
    
- **Method:** Instead of only increasing power to one side, we use **Differential Thrust** to keep the total altitude stable.
    

---

### File 3: `Drone_Motor_Mixer.md`

**Description:** Translating logic and PID into motor outputs.

## 1. Input Values

- **Base Thrust ($T_{base}$):** Derived from `State_Machine.md` physics.
    
- **Correction ($C$):** Derived from `PID_Control.md`.
    

## 2. Mixer Equations (Roll Example)

For a drone to stay balanced while performing its state (Up/Down/Hover):

- **Left Motors ($M_{left}$):** $T_{base} + C$
    
- **Right Motors ($M_{right}$):** $T_{base} - C$
    

## 3. Physics Verification

By using $+C$ and $-C$, the **Total Thrust** remains constant:

$$\sum T = (T_{base} + C) + (T_{base} - C) = 2 \cdot T_{base}$$

This prevents the drone from "jumping" or gaining altitude during stabilization.