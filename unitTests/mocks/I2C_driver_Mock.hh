#include <I2C_driver.hh>
#include <queue>

namespace Drivers{

struct I2Cdata{
	uint16_t addr;
	std::vector<uint8_t> data;
};

class I2C : public I2Cifc{
	std::queue<I2Cdata> read;
	std::queue<I2Cdata> write;
public:
	I2C();
	~I2C();
	const std::vector<uint8_t> Read(const uint16_t& addres, const unsigned int size) override;
	void Write(const uint16_t& addres, const std::vector<uint8_t>& data) override;
	bool clearBuffor();
	void setReadingBuffor(const std::vector<I2Cdata>& data);
	const I2Cdata getWritieLine();
};

std::shared_ptr<I2C> createDriver();

}