#include "GPIO_driver_Mock.hh"

namespace Drivers{

std::map<unsigned int, std::shared_ptr<GPIO>> testHelperMapGPIO;

std::map<unsigned int, std::shared_ptr<GPIO>>& createDriverGPIO(const std::vector<unsigned int>& pins){
	testHelperMapGPIO.clear();
	for(const auto& pin : pins)
		testHelperMapGPIO[pin].reset(new GPIO(pin));
	return testHelperMapGPIO;
}

std::shared_ptr<GPIOifc> makeGPIO(const unsigned int& pinNr, const bool&){
	try{
		return testHelperMapGPIO.at(pinNr);
	}catch(...){
		throw std::invalid_argument("Pin nr: " + std::to_string(pinNr) + " is not initialized.");
	}
}

GPIO::GPIO(unsigned int _id):
	id(_id){}

GPIO::~GPIO(){}

bool GPIO::state(){
	if(getState.empty())
		throw std::invalid_argument("There is no state to read for pin nr: " + std::to_string(id));
	bool toReturn = getState.front();
	getState.pop();
	return toReturn;
}

GPIO::operator bool(){
	return state();
}

void GPIO::operator=(const bool& state){
	setState.push_back(state);
}

bool GPIO::clearBuffor(){
	if(getState.empty() && setState.empty())
		return true;
	while(!getState.empty())
		getState.pop();
	setState.clear();
	return false;
}

void GPIO::setPinReading(const std::vector<bool>& data){
	for(const auto& elem : data)
		getState.push(elem);
}

const std::vector<bool> GPIO::getPinSetting(){
	auto toReturn = setState;
	setState.clear();
	return toReturn;
}

}
