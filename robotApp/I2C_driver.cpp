#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <I2C_driver.hh>

namespace Drivers{

class I2C : public I2Cifc{
	int file_i2c;
	unsigned char buffer[100];
	const unsigned int maxLength=100;
public:
	I2C(const std::string& id, const uint8_t addr){ //"/dev/i2c-1"
		const char *filename = id.c_str();
		if ((file_i2c = open(filename, O_RDWR)) < 0)
			throw std::invalid_argument("Cannot write into " + id);
		if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
			throw std::invalid_argument("Cannot write into " + id + " to addres: " + std::to_string(addr));
	}
	~I2C(){
		close(file_i2c);
	}
	const std::vector<uint8_t> Read(const uint16_t& addres, const unsigned int size) override{
		std::vector<uint8_t> toReturn;
		if(size > maxLength)
			throw std::out_of_range("Size of data recived via i2c is bigger than allowed" + std::to_string(maxLength));
		buffer[0] = (addres >> 8) & 0xff;
		buffer[1] = addres & 0xff;
		if (write(file_i2c, buffer, 2) != 2)
			throw std::invalid_argument("Failed to send addres during reading from the i2c bus.\n");
		if (read(file_i2c, buffer, size) != (ssize_t)size)
			throw std::invalid_argument("Failed to read from the i2c bus.\n");
		for(unsigned int i = 0; i < size; i++)
			toReturn.push_back(buffer[i]);
		return toReturn;
	}
	void Write(const uint16_t& addres, const std::vector<uint8_t>& data) override{
		if(2 + data.size() > maxLength)
			throw std::out_of_range("Size of data transfer via i2c is bigger than allowed" + std::to_string(maxLength));
		buffer[0] = (addres >> 8) & 0xff;
		buffer[1] = addres & 0xff;
		for(unsigned int i = 0; i < data.size(); i++){
			buffer[i + 2] = data[i];
		}
		if (write(file_i2c, buffer, data.size() + 2) != (ssize_t)(data.size() + 2))
			throw std::invalid_argument("Failed to write to the i2c bus.\n");
	}
};

std::shared_ptr<I2Cifc> makeI2C(const std::string& devicePath, const uint8_t addr){
    std::shared_ptr<I2Cifc> toReturn(new I2C(devicePath, addr));
	return toReturn;
}

}