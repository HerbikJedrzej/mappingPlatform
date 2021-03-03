#pragma once
#include "StrategySPI_ifc.hh"

namespace StrategySpi{

class PositionStrategy : public StrategySpiIfc{
    double* actualPositionX;
    double* actualPositionY;
    double* actualRotation;
    double* expectedPositionX;
    double* expectedPositionY;
    double* expectedRotation;
    bool* applyPosition;
public:
    PositionStrategy(
        double* _actualPositionX,
        double* _actualPositionY,
        double* _actualRotation,
        double* _expectedPositionX,
        double* _expectedPositionY,
        double* _expectedRotation,
        bool* _applyPosition = nullptr
    );
    bool set(StrategySpiData& data) override;
    bool get(StrategySpiData& data) override;
    unsigned int getSize() override;
    std::string getName() override;
};

}