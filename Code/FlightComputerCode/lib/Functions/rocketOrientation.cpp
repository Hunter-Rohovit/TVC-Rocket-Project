//This file is to ensure the IMU's orientation is correct relative to the rocket
//It makes sure to apply the necessary transformation from the IMU's actual reference frame to the body's reference frame
//It also makes sure positive and negative, pitch/yaw is correct relative to the rocket body's reference frame
//This file is essential to ensure no critical errors are made in the flight code with orientation

#include <rocketOrientation.h>
#include <QuatToEul.h>

void setupIMUs(){
    if(!IMU1.begin(0x4A, Wire)){
        Serial.println("IMU1 failed to initialize");
    }
    else{
        Serial.println("IMU1 Initialized");
    }
    /*if(!IMU2.begin(0x4A, Wire1)){
        Serial.println("IMU2 failed to initialize");
    }
    else{
        Serial.println("IMU2 Initialized");
    } */

}

static inline Quat makeFrameC(){
    const float s = 0.70710678118f; // sin(±45°) = cos(±45°)
    Quat qYm90 = { s, 0.0f, -s, 0.0f }; // -90° about Y
    Quat qZp90 = { s, 0.0f,  0.0f,  s }; // +90° about Z
    Quat C = qMul(qZp90, qYm90);
    qNormalize(C);
    return C;
}

Quat IMU1toInertial(const Quat& qIMU1){
    static const Quat C = makeFrameC();     // sensor→inertial axes change
    Quat qI_to_W = qMul(qIMU1, qConj(C));
    qNormalize(qI_to_W);
    return qI_to_W;
}

Quat IMU2toBody(const Quat& qIMU2);

// wraps the euler angles about 180 degrees due to any offset errors caused by euler conversions
static inline float wrap180(float a){
    while (a <= -180.0f) a += 360.0f;
    while (a >   180.0f) a -= 360.0f;
    return a;
}

void tvcMovementTest(){
    setupIMUs();
    Serial.println("IMU setup complete");
    IMU1.enableReports(SH2_GAME_ROTATION_VECTOR, 400);

    while (true){
        delay(10);
        if (IMU1.dataAvailable()) {
            sh2_SensorValue_t sensor = IMU1.getSensorValues();

            if (sensor.sensorId == SH2_GAME_ROTATION_VECTOR) {
                Quat qIMU1 = {
                    sensor.un.gameRotationVector.real,
                    sensor.un.gameRotationVector.i,
                    sensor.un.gameRotationVector.j,
                    sensor.un.gameRotationVector.k
                };

                // Transform to inertial frame
                Quat qInertial = IMU1toInertial(qIMU1);

                // Convert to Euler
                EulerAngles eulerAngles = convertQuatToEul(
                    qInertial.w, qInertial.x, qInertial.y, qInertial.z
                );
                Serial.print("  Roll: ");
                Serial.print(eulerAngles.roll, 4);
                Serial.print("  Pitch: ");
                Serial.print(eulerAngles.pitch, 4);
                Serial.print("  Yaw: ");
                Serial.println(eulerAngles.yaw, 4);
            }
        } 
    }
}

