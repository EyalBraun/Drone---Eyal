#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdint.h>
#include <math.h>

// --- Physical Constants (מתוך הקבצים שלך) ---
const float GRAVITY = 9.81f;
const float MASS = 0.5f;
const float TARGET_ACCEL = 0.33f;
const float HOVER_FORCE = 4.905f;      // F = m*g
const float UP_THRUST_MAX = 5.07f;     // m(g+a)
const float BRAKE_FORCE_SMOOTH = 4.855f; //
const float RAMP_INCREMENT = 1.26f;    // 5.07N / 4s

// --- State Machine Definitions ---
uint8_t engineControl = 0x00;
#define BIT_HOVER (1 << 0)
#define BIT_DOWN  (1 << 1)
#define BIT_UP    (1 << 2)
#define BIT_ARM   (1 << 3)

// --- Kalman Filter Struct ---
struct Kalman {
    float angle; float bias; float P[2][2];
};

void UpdateKalman(Kalman* k, float newAngle, float newRate, float dt) {
    float Q_angle = 0.001f; float Q_bias = 0.003f; float R_measure = 0.03f;
    k->angle += dt * (newRate - k->bias);
    k->P[0][0] += dt * (dt * k->P[1][1] - k->P[0][1] - k->P[1][0] + Q_angle);
    k->P[0][1] -= dt * k->P[1][1]; k->P[1][0] -= dt * k->P[1][1]; k->P[1][1] += Q_bias * dt;
    float S = k->P[0][0] + R_measure;
    float K[2] = { k->P[0][0] / S, k->P[1][0] / S };
    float y = newAngle - k->angle;
    k->angle += K[0] * y; k->bias += K[1] * y;
    float P00_temp = k->P[0][0]; float P01_temp = k->P[0][1];
    k->P[0][0] -= K[0] * P00_temp; k->P[0][1] -= K[0] * P01_temp;
    k->P[1][0] -= K[1] * P00_temp; k->P[1][1] -= K[1] * P01_temp;
}

int main() {
    InitWindow(1280, 720, "Drone 1.0v - Final Integrated Architecture");
    Camera3D camera = { { 10.0f, 10.0f, 10.0f }, { 0, 1.0f, 0 }, { 0, 1.0f, 0 }, 45.0f, CAMERA_PERSPECTIVE };

    Vector3 dronePos = { 0, 0.5f, 0 }, droneVel = { 0, 0, 0 }, droneRot = { 0, 0, 0 }, angVel = { 0, 0, 0 };
    Kalman kPitch = { 0 }, kRoll = { 0 };

    float currentThrust = 0.0f;
    float Kp = 1.8f; // "The Muscle"
    float Kd = 0.9f; // "The Brake"

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 1. Input Logic
        if (IsKeyPressed(KEY_A)) engineControl ^= BIT_ARM;
        if (engineControl & BIT_ARM) {
            engineControl &= ~(BIT_UP | BIT_DOWN | BIT_HOVER);
            if (IsKeyDown(KEY_W))      engineControl |= BIT_UP;
            else if (IsKeyDown(KEY_S)) engineControl |= BIT_DOWN;
            else                       engineControl |= BIT_HOVER;
        }
        else { engineControl = 0; currentThrust = 0; }

        // 2. Thrust & Thermal Protection (Slew Rate)
        float targetThrust = 0.0f;
        if (engineControl & BIT_ARM) {
            if (engineControl & BIT_UP) {
                targetThrust = UP_THRUST_MAX;
            }
            else if (engineControl & BIT_DOWN) {
                targetThrust = 4.0f; //
            }
            else if (engineControl & BIT_HOVER) {
                // Method B Brake
                if (droneVel.y > 0.05f) targetThrust = BRAKE_FORCE_SMOOTH;
                else targetThrust = HOVER_FORCE;
            }

            // Slew Rate Implementation: Ramp force over 4 seconds
            if (currentThrust < targetThrust) {
                currentThrust += RAMP_INCREMENT * dt;
                if (currentThrust > targetThrust) currentThrust = targetThrust;
            }
            else if (currentThrust > targetThrust) {
                currentThrust -= RAMP_INCREMENT * dt; // Smooth transition down
                if (currentThrust < targetThrust) currentThrust = targetThrust;
            }
        }

        // 3. Stability Logic (Kalman + PID)
        if (engineControl & BIT_ARM) {
            // Simulate noise and update Kalman
            UpdateKalman(&kPitch, droneRot.x + (GetRandomValue(-5, 5) / 100.0f), angVel.x, dt);
            UpdateKalman(&kRoll, droneRot.z + (GetRandomValue(-5, 5) / 100.0f), angVel.z, dt);

            // PID Correction
            angVel.x += (Kp * (0.0f - kPitch.angle) - Kd * angVel.x) * dt;
            angVel.z += (Kp * (0.0f - kRoll.angle) - Kd * angVel.z) * dt;
        }

        // 4. Physics Integration
        float netYAccel = (engineControl & BIT_ARM) ? ((currentThrust - (MASS * GRAVITY)) / MASS) : -GRAVITY;
        droneVel.y += netYAccel * dt;
        dronePos.y += droneVel.y * dt;
        droneRot.x += angVel.x * dt;
        droneRot.z += angVel.z * dt;

        if (dronePos.y < 0.5f) { dronePos.y = 0.5f; droneVel.y = 0; angVel = { 0 }; droneRot = { 0 }; }

        // 5. Rendering
        BeginDrawing();
        ClearBackground(GetColor(0x111111FF));
        BeginMode3D(camera);
        DrawGrid(20, 1.0f);
        rlPushMatrix();
        rlTranslatef(dronePos.x, dronePos.y, dronePos.z);
        rlRotatef(droneRot.z * RAD2DEG, 0, 0, 1); rlRotatef(droneRot.x * RAD2DEG, 1, 0, 0);
        DrawCube({ 0,0,0 }, 0.4f, 0.2f, 0.4f, DARKGRAY);
        rlPushMatrix(); rlRotatef(45, 0, 1, 0); DrawCube({ 0,0,0 }, 2.5f, 0.05f, 0.1f, GRAY); rlPopMatrix();
        rlPushMatrix(); rlRotatef(-45, 0, 1, 0); DrawCube({ 0,0,0 }, 2.5f, 0.05f, 0.1f, GRAY); rlPopMatrix();
        if (engineControl & BIT_ARM) {
            for (int i = 0; i < 4; i++) DrawCircle3D({ (i < 2 ? 0.8f : -0.8f), 0.1f, (i % 2 ? 0.8f : -0.8f) }, 0.4f, { 0,1,0 }, 90.0f, Fade(SKYBLUE, 0.4f));
        }
        rlPopMatrix();
        EndMode3D();

        DrawRectangle(10, 10, 350, 180, Fade(BLACK, 0.8f));
        DrawText(TextFormat("THRUST: %.3f N / %.3f N", currentThrust, targetThrust), 20, 20, 18, YELLOW);
        DrawText(TextFormat("ALTITUDE: %.2f m", dronePos.y), 20, 50, 18, WHITE);
        DrawText(TextFormat("KALMAN PITCH: %.4f", kPitch.angle), 20, 80, 18, LIME);
        DrawText(TextFormat("STATE: %s", (engineControl & BIT_UP) ? "UP (RAMPING)" : (engineControl & BIT_HOVER) ? "HOVER" : "IDLE"), 20, 110, 18, SKYBLUE);
        EndDrawing();
    }
    CloseWindow(); return 0;
}