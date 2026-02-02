## 1. Control Byte (8-bit Integer)

The system is controlled by a single byte (`uint8_t engineControl`).

| **Bit Index** | **Name**  | **Function**                                      |
| ------------- | --------- | ------------------------------------------------- |
| **Bit 0**     | **HOVER** | `1`: Maintain altitude ($T = W = 4.905\text{N}$). |
| **Bit 1**     | **DOWN**  | `1`: Controlled descent ($T < W$).                |
| **Bit 2**     | **UP**    | `1`: Ascent/Acceleration ($T > W$).               |
| **Bit 3**     | **ARM**   | **Master Switch**. If `0`, all motors = `0`.      |

## 2. Logic Constraints

- **Safety First:** If Bit 3 is `0`, all other bits are ignored.
    
- **Mutual Exclusion:** Bits 0, 1, and 2 cannot be `1` simultaneously.
    
- **Update Frequency:** The state should be polled at 100Hz, while the PID loop (see below) runs at 400Hz.