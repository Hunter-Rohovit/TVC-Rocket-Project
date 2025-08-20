#include <commandToActualTransformation.h>

float evaluatePoly(String axis_direction, float servoAngle) {
    const float* coeffs;
    if(axis_direction == "pitchUp"){
        coeffs = pitchUpCoeffs;

    }
    else if (axis_direction == "pitchDown"){
        coeffs = pitchDownCoeffs;

    }
    else if (axis_direction == "yawUp"){
        coeffs = yawUpCoeffs;
    }
    else{
        coeffs = yawDownCoeffs;
    }

    float result = 0;
    for (int i = 0; i <= 6; i++) {
        result += coeffs[i] * pow(servoAngle, 6 - i);
    }
    return result;
}