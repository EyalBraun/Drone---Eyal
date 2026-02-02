### **Phase 1: Acceleration to Target Speed**

- **Target:** Reach $1 \text{ m/s}$ in $3 \text{ seconds}$.
    
- **Required Acceleration ($a$):** $1 / 3 = 0.33 \text{ m/s}^2$.
    
- **Total Thrust Needed ($F_{accel}$):**
    
    $$F_t = m(g + a) = 0.5 \cdot (9.81 + 0.33) = 5.07 \text{ N}$$
    

### **Phase 2: Thermal Protection (Slew Rate)**

Since we cannot jump from $0 \text{ N}$ to $5.07 \text{ N}$ instantly without damaging the ESCs/Engines, we ramp the force over $4 \text{ seconds}$.

- **Ramp Increment:** $\frac{5.07 \text{ N}}{4 \text{ s}} = 1.26 \text{ N}$ added every second.
    

### **Phase 3: Transition to Constant Velocity (Newton's 1st Law)**

Once the drone reaches $1 \text{ m/s}$, you must stop accelerating. To make $a = 0$, the $\sum F$ must be $0$.

- **Required Thrust ($F_{cruise}$):**
    
    $$F_t = W = m \cdot g = 0.5 \cdot 9.81 = 4.90 \text{ N}$$
    
- **Transition:** Slowly decrease the force from $5.07 \text{ N}$ down to $4.90 \text{ N}$ over a short period to avoid a "jerk" in motion.
    

---

### **Summary of the Force Profile**

| **Time**     | **Goal**     | **Force (Ft​)**                   | **Result**                              |
| ------------ | ------------ | --------------------------------- | --------------------------------------- |
| **0s to 4s** | Protection   | Ramp from $0$ to $5.07 \text{ N}$ | Drone begins lifting off                |
| **4s to 7s** | Speed        | Hold at $5.07 \text{ N}$          | Drone accelerates to $1 \text{ m/s}$    |
| **At 7s+**   | Constant $v$ | Drop to $4.90 \text{ N}$          | Drone moves at constant $1 \text{ m/s}$ |
|              |              |                                   |                                         |