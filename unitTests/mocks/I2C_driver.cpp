#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <I2C_driver.hh>
#include "I2C_driver_Mock.hh"

namespace Drivers{

static std::shared_ptr<I2C> testHelperPointerI2C = {nullptr};

std::shared_ptr<I2C> createDriver(){
	testHelperPointerI2C.reset(new I2C);
	return testHelperPointerI2C;
}

std::shared_ptr<I2Cifc> makeI2C(const std::string&, const uint8_t){
	if(!testHelperPointerI2C)
		throw std::invalid_argument("testHelperPointerI2C is not initialized.");
	return testHelperPointerI2C;
}

I2C::I2C(){}

I2C::~I2C(){}

const std::vector<uint8_t> I2C::Read(const uint16_t& addres, const unsigned int size){
	if(read.empty())
		throw std::invalid_argument("There is no more lines to read.");
	auto toReturn = read.front();
	read.pop();
	if(toReturn.data.size() != size)
		throw std::invalid_argument("Line to read has size = " + std::to_string(toReturn.data.size()) + ". Expected = " + std::to_string(size) + ".");
	if(toReturn.addr != addres)
		throw std::invalid_argument("Line to read has address = " + std::to_string(toReturn.addr) + ". Expected = " + std::to_string(addres) + ".");
	return toReturn.data;
}

void I2C::Write(const uint16_t& addres, const std::vector<uint8_t>& data){
	I2Cdata line{addres, data};
	write.push(line);
}

bool I2C::clearBuffor(){
	if(write.empty() && read.empty())
		return true;
	while(!write.empty())
		write.pop();
	while(!read.empty())
		read.pop();
	return false;
}

void I2C::setReadingBuffor(const std::vector<I2Cdata>& data){
	for(const auto& elem : data){
		read.push(I2Cdata{elem.addr, elem.data});
	}
}

const I2Cdata I2C::getWritieLine(){
	if(write.empty())
		return I2Cdata{0, {}};
	auto toReturn = write.front();
	write.pop();
	return toReturn;
}

}