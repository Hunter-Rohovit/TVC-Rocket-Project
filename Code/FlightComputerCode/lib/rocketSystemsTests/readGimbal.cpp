#include <readGimbal.h>


float readGimbalAngle(const char* axis) {
    if (IMU2.dataAvailable()) {
        sh2_SensorValue_t sensor = IMU2.getSensorValues();
        if (sensor.sensorId == SH2_GAME_ROTATION_VECTOR) {
            float q0 = sensor.un.gameRotationVector.real;
            float q1 = sensor.un.gameRotationVector.i;
            float q2 = sensor.un.gameRotationVector.j;
            float q3 = sensor.un.gameRotationVector.k;

            EulerAngles angles = convertQuatToEul(q0, q1, q2, q3);

            if (strcmp(axis, "pitch") == 0) return angles.pitch;
            if (strcmp(axis, "yaw") == 0)   return angles.yaw;
        }
    }
    return NAN;
}

float readPitch() {
    if (IMU2.dataAvailable()) {
        sh2_SensorValue_t sensor = IMU2.getSensorValues();
        if (sensor.sensorId == SH2_GAME_ROTATION_VECTOR) {
            EulerAngles angles = convertQuatToEul(
                sensor.un.gameRotationVector.real,
                sensor.un.gameRotationVector.i,
                sensor.un.gameRotationVector.j,
                sensor.un.gameRotationVector.k
            );
            return angles.pitch;
        }
    }
    return NAN;
}
float readYaw() {
    if (IMU2.dataAvailable()) {
        sh2_SensorValue_t sensor = IMU2.getSensorValues();
        if (sensor.sensorId == SH2_GAME_ROTATION_VECTOR) {
            EulerAngles angles = convertQuatToEul(
                sensor.un.gameRotationVector.real,
                sensor.un.gameRotationVector.i,
                sensor.un.gameRotationVector.j,
                sensor.un.gameRotationVector.k
            );
            return angles.yaw;
        }
    }
    return NAN;
}