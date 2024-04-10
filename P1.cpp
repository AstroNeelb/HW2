#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 1.1
class Sensor {
public:
    virtual void gatherData() const = 0;
    virtual void processData() const = 0;
    virtual ~Sensor() {} // Adding a virtual destructor for polymorphic behavior
};

// 1.2
class Airspeed : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Airspeed Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Airspeed Sensor" << endl;
    }
};

class Attitude : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Attitude Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Attitude Sensor" << endl;
    }
};

class Altimeter : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Altimeter Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Altimeter Sensor" << endl;
    }
};

//1.3
class SensorFactory {
public:
    static Sensor* createSensor(const string& sensorType) {
        if (sensorType == "Airspeed") {
            return new Airspeed();
        }
        else if (sensorType == "Attitude") {
            return new Attitude();
        }
        else if (sensorType == "Altimeter") {
            return new Altimeter();
        }
        else {
            return nullptr;
        }
    }
};

//1.4
class AerospaceControlSystem {
private:
    vector<Sensor*> sensors;

public:
    void addSensor(Sensor* sensor) {
        sensors.push_back(sensor);
    }

    void monitorAndAdjust() {
        for (Sensor* sensor : sensors) {
            sensor->gatherData();
            sensor->processData();
            cout << "Adjusting controls based on sensor data." << endl;
        }
    }

    ~AerospaceControlSystem() {
        for (Sensor* sensor : sensors) {
            delete sensor;
        }
    }
};

// 1.5
int main() {
    AerospaceControlSystem ctrlSys;

    Sensor* airspeedSensor = SensorFactory::createSensor("Airspeed");
    Sensor* attitudeSensor = SensorFactory::createSensor("Attitude");
    Sensor* altimeterSensor = SensorFactory::createSensor("Altimeter");

    ctrlSys.addSensor(airspeedSensor);
    ctrlSys.addSensor(attitudeSensor);
    ctrlSys.addSensor(altimeterSensor);

    ctrlSys.monitorAndAdjust();

    return 0;
}
