#include "SPI_driver_Mock.hh"

namespace Drivers{

static std::shared_ptr<SPI> testHelperPointerSPI = {nullptr};

std::shared_ptr<SPI> createDriverSPI(){
	testHelperPointerSPI.reset(new SPI);
	return testHelperPointerSPI;
}

std::shared_ptr<SPIifc> makeSPI(const std::string&){
	if(!testHelperPointerSPI)
		throw std::invalid_argument("testHelperPointerSPI is not initialized.");
	return testHelperPointerSPI;
}

SPI::SPI(){}

SPI::~SPI(){}

bool SPI::transfer(uint8_t* dataSend, uint8_t* dataRecive, const unsigned int& length){
	if(recive.empty())
		throw std::invalid_argument("There is no more lines to recive.");
	auto dataToPop = recive.front();
	recive.pop();
	if(dataToPop.size() != length)
		throw std::invalid_argument("Line to SPI recive has size = " + std::to_string(dataToPop.size()) + ". Expected = " + std::to_string(length) + ".");
	std::vector<uint8_t> dataToPush;
	for(unsigned int i = 0; i < length; i++){
		dataToPush.push_back(dataSend[i]);
		dataRecive[i] = dataToPop[i];
	}
	send.push(dataToPush);
	return true;
}

bool SPI::clearBuffor(){
	if(send.empty() && recive.empty())
		return true;
	while(!send.empty())
		send.pop();
	while(!recive.empty())
		recive.pop();
	return false;
}

void SPI::setReciveBuffor(const std::vector<std::vector<uint8_t>>& data){
	for(auto elem : data){
		recive.push(elem);
	}
}

const std::vector<uint8_t> SPI::getSendLine(){
	if(send.empty())
		return std::vector<uint8_t>{{}};
	auto toReturn = send.front();
	send.pop();
	return toReturn;
}

}