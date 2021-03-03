#include <SPI_driver.hh>
#include <queue>

namespace Drivers{

class SPI : public SPIifc{
	std::queue<std::vector<uint8_t>> recive;
	std::queue<std::vector<uint8_t>> send;
public:
	SPI();
	~SPI();
	bool transfer(uint8_t* dataSend, uint8_t* dataRecive, const unsigned int& length) override;
	bool clearBuffor();
	void setReciveBuffor(const std::vector<std::vector<uint8_t>>& data);
	const std::vector<uint8_t> getSendLine();
};

std::shared_ptr<SPI> createDriverSPI();

}