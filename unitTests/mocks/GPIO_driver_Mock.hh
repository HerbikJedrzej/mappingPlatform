#include <GPIO_driver.hh>
#include <vector>
#include <queue>
#include <map>

namespace Drivers{

class GPIO : public GPIOifc{
	const unsigned int id;
	std::vector<bool> setState;
	std::queue<bool> getState;
public:
	GPIO(unsigned int);
	~GPIO();
	bool state() override;
    operator bool() override;
    void operator=(const bool& state) override;
	bool clearBuffor();
	void setPinReading(const std::vector<bool>& data);
	const std::vector<bool> getPinSetting();
};

std::map<unsigned int, std::shared_ptr<GPIO>>& createDriverGPIO(const std::vector<unsigned int>& pins);

}
