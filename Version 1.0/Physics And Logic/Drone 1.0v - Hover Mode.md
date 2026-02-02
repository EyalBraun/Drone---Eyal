

**Goal:** Transition from upward motion ($1 \text{ m/s}$) to a stable, constant-altitude "levitation."

## 1. Core Physics: Newton’s First Law

To remain at a fixed point in space, the drone must satisfy two conditions:

1. **Velocity ($v$) must be $0 \text{ m/s}$.**
    
2. **Net Force ($\sum F$) must be $0 \text{ N}$.**
    

If $\sum F = 0$ while the drone is already moving, it will keep moving. Therefore, we must **brake** first, then **hover**.

---

## 2. Execution Method A: The "Free-Fall" Stop

This method relies entirely on gravity ($g$) to provide the deceleration.

- **Logic:** Temporarily set Thrust ($F_t$) to $0$.
    
- **Deceleration ($a$):** $-9.81 \text{ m/s}^2$ (pure gravity).
    
- **Time Calculation:**
    
    $$t = \frac{v}{g} = \frac{1 \text{ m/s}}{9.81 \text{ m/s}^2} \approx 0.1 \text{ sec}$$
    
- **Action:** Turn motors OFF for exactly **$0.1$ seconds**, then immediately switch to Hover Mode.
    

---

## 3. Execution Method B: The Controlled Brake (Active Deceleration)

This is the "Architect's Choice" for stability. Instead of turning engines off, we reduce thrust to a level slightly below the drone's weight.

- **Scenario:** We want a smooth stop over **$10$ seconds**.
    
- **Required Deceleration ($a$):** $0.1 \text{ m/s}^2$.
    
- **Force Calculation ($F_t$):**
    
    $$F_t = m(g - a)$$
    
    $$F_t = 0.5 \text{ kg} \cdot (9.81 - 0.1) = 4.855 \text{ N}$$
    
- **Action:** Maintain thrust at **$4.855 \text{ N}$** for **$10$ seconds**. Once $v = 0$, switch to Hover Mode.
    

---

## 4. The Hover State (Levitation)

Once the Braking Phase (Method A or B) brings the velocity to zero, the ESP32 must enter the **Static Equilibrium** state.

- **Condition:** $F_t = |W|$ (Thrust equals Weight).
    
- **Formula:**
    
    $$F_t = m \cdot g = 0.5 \text{ kg} \cdot 9.81 \text{ m/s}^2 = 4.905 \text{ N}$$
    
- **Result:** $\sum F = 0$. Since the drone stopped at the end of the braking phase, it will now remain at that exact height indefinitely (or until the Hover button is released).
    

---

### Architect's Summary Table

|**Phase**|**Goal**|**Thrust (Ft​)**|**Duration**|
|---|---|---|---|
|**Braking (Fast)**|$v \to 0$|$0 \text{ N}$|$0.1 \text{ s}$|
|**Braking (Smooth)**|$v \to 0$|$4.855 \text{ N}$|$10 \text{ s}$|
|**Hovering**|Maintain Height|$4.905 \text{ N}$|Continuous|