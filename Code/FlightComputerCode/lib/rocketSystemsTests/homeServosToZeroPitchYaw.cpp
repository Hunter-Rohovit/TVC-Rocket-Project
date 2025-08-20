#include <homeServosToZeroPitchYaw.h>


void homeServosToZeroPitchYaw() {

    while(!IMU2.begin(0x4A, Wire1)) {
        Serial.println("Failed to initialize IMU2");
        delay(20);
    }
    IMU2.enableReports("SH2_GAME_ROTATION_VECTOR", 400);
    Serial.println("TVC mount IMU initialized");
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo1.setLimits(80, -30, 30);
    servo2.setLimits(80, -30, 30);
    servo1.setPosition(servo1.zeroPosition);
    servo2.setPosition(servo2.zeroPosition);

    Serial.println("Homing servos to pitch ≈ 0 and yaw ≈ 0...");

    float tolerance = 0.15;
    float pitch, yaw;

    while (true) {
    if (IMU2.dataAvailable()) {
        sh2_SensorValue_t sensor = IMU2.getSensorValues();

        if (sensor.sensorId == SH2_GAME_ROTATION_VECTOR) {
            float q0 = sensor.un.gameRotationVector.real;
            float q1 = sensor.un.gameRotationVector.i;
            float q2 = sensor.un.gameRotationVector.j;
            float q3 = sensor.un.gameRotationVector.k;
            EulerAngles angles = convertQuatToEul(q0, q1, q2, q3);
            pitch = angles.pitch;
            yaw = angles.yaw;

            Serial.print("Yaw: "); Serial.println(yaw);
            Serial.print("Pitch: "); Serial.println(pitch);

            // Adjust Yaw first
            if (yaw > tolerance) {
                Serial.println("Yaw too high. Lowering Yaw");
                Serial.print("Current Position: "); Serial.println(servo2.servoPosition);
                servo2.setPosition(servo2.servoPosition + 1);
                Serial.print("New Position: "); Serial.println(servo2.servoPosition);
            }
            else if (yaw < -tolerance) {
                Serial.println("Yaw too low. Raising Yaw");
                Serial.print("Current Position: "); Serial.println(servo2.servoPosition);
                servo2.setPosition(servo2.servoPosition - 1);
                Serial.print("New Position: "); Serial.println(servo2.servoPosition);
            }
            // If Yaw is within tolerance, adjust PITCH
            else if (pitch > tolerance) {
                Serial.println("Pitch too high. Lowering Pitch");
                Serial.print("Current Position: "); Serial.println(servo1.servoPosition);
                servo1.setPosition(servo1.servoPosition - 1);
                Serial.print("New Position: "); Serial.println(servo1.servoPosition);
            }
            else if (pitch < -tolerance) {
                Serial.println("Pitch too low. Raising Pitch");
                Serial.print("Current Position: "); Serial.println(servo1.servoPosition);
                servo1.setPosition(servo1.servoPosition + 1);
                Serial.print("New Position: "); Serial.println(servo1.servoPosition);
            }
            else {
                Serial.println("Gimbal aligned with zero pitch and yaw.");
                break; // Exit once both pitch and yaw are within tolerance
            }
        }
    }
    delay(200);
}
    // Capture current positions and set as new home
    int servo1_zero = servo1.servoPosition;
    int servo2_zero = servo2.servoPosition;

    servo1.setLimits(servo1_zero, -30, 30);
    servo2.setLimits(servo2_zero, -30, 30);

    Serial.print("New servo1 zero: "); Serial.println(servo1_zero);
    Serial.print("New servo2 zero: "); Serial.println(servo2_zero);
}