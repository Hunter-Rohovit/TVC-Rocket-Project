#include <BNO085.h>

BNO085::BNO085():
    bno(-1){}

bool BNO085::begin(uint8_t address, TwoWire &wirePort){
    if(!bno.begin_I2C(address, &wirePort)){
        return false;
    }
    return true;
}

bool BNO085::enableReports(String reportType, uint32_t frequencyHz){
    uint32_t interval_us = 1000000UL/frequencyHz; //interval is required in micro seconds
    if (reportType == "SH2_GAME_ROTATION_VECTOR"){
       return bno.enableReport(SH2_GAME_ROTATION_VECTOR, interval_us);
    }
    else if (reportType == "SH2_ROTATION_VECTOR"){
        return bno.enableReport(SH2_ROTATION_VECTOR, interval_us);
    }
    else if (reportType == "SH2_CAL_GYRO"){
        return bno.enableReport(SH2_GYROSCOPE_CALIBRATED, interval_us);
    }
    else if (reportType == "SH2_ACCELEROMETER"){
        return bno.enableReport(SH2_ACCELEROMETER, interval_us);
    }
    else if (reportType == "SH2_GRAVITY"){
        return bno.enableReport(SH2_GRAVITY, interval_us);
    }
    else if (reportType == "SH2_MAGNETIC_FIELD_CALIBRATED"){
        return bno.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED, interval_us);
    }
    else {
        Serial.println("Unknown report type");
        return false;
    }
}

bool BNO085::dataAvailable(){
    return bno.getSensorEvent(&sensorValue);
}

sh2_SensorValue_t BNO085::getSensorValues() {
    return sensorValue;
}


