#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <GPIO_driver.hh>

namespace Drivers{

class GPIO : public GPIOifc{
	bool s;
	bool output;
    int val;
    int dir;
public:
	GPIO(const unsigned int& pinNr, const bool& isOutput):
		s(false),
		output(isOutput),
		val(open(("/sys/class/gpio/gpio" + std::to_string(pinNr) + "/value").c_str(), output? O_WRONLY : O_RDONLY)),
		dir(open(("/sys/class/gpio/gpio" + std::to_string(pinNr) + "/direction").c_str(), O_WRONLY))
	{
		if(val == -1)
			throw std::invalid_argument("Cannot open /sys/class/gpio/gpio" + std::to_string(pinNr) + "/value");
		if(dir == -1)
			throw std::invalid_argument("Cannot open /sys/class/gpio/gpio" + std::to_string(pinNr) + "/direction");
		ssize_t size =  output? 3 : 2;
		if (write(dir, output? "out" : "in", size) != size)
			throw std::invalid_argument("Cannot write into /sys/class/gpio/gpio" + std::to_string(pinNr) + "/direction");
	}
	~GPIO(){
		close(val);				    
		close(dir);				    
	};
	bool state() override{
		if(output)
		    return s;
		char value;
		lseek(val, 0, SEEK_SET);
		if(read(val, &value, 1) != 1)
			throw std::invalid_argument("Cannot read from /sys/class/gpio/gpio/value");
		return (value == '1');
	}
    operator bool() override{
		return state();
	}
    void operator=(const bool& state) override{
		if(!output)
			throw std::invalid_argument("GPIO is defined as input.");
		s = state;
		if (write(val, s? "1" : "0", 1) != 1)
			throw std::invalid_argument("Cannot write into /sys/class/gpio/gpio/value");
	}
};

std::shared_ptr<GPIOifc> makeGPIO(const unsigned int& pinNr, const bool& isOutput){
	std::shared_ptr<GPIOifc> toReturn(new GPIO(pinNr, isOutput));
	return toReturn;
}

}
