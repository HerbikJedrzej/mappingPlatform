#include <iostream>
#include <logSystem.hh>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <factory.hh>

class STMlog{
public:
    STMlog(const std::string& path, const std::string& name = "stmLogs.log"){
        system(std::string("cat /dev/ttyS0 > " + path + "/" + name + " &").c_str());
    }
    ~STMlog(){
        system("sudo kill $(ps aux | grep 'cat /dev/ttyS0' | awk '{print $2}') &");
    }
};

class FinishAfterEnterButton{
	bool flag;
	std::thread thread_obj;
public:
	FinishAfterEnterButton():
		flag(true),
		thread_obj(std::thread(
			[](bool* flag)->void{
				std::cin.get();
				*flag = false;
			}
			, &flag
		)){}
	~FinishAfterEnterButton(){
		thread_obj.join();
	}
	bool checkFlag(){
		return flag;
	}
};

int main(int argc, char **argv){
	if((argc != 1) && (argc != 3)){
		std::cout << "ERROR: program epect two or none argument. Exaple: ./robotApp ./logsPath ./configsPath" << std::endl;
		return 0;
	}
	std::string logsPath((argc == 3)? argv[1] : ".");
	std::string configPath((argc == 3)? argv[2] : "./configs");
	Log log(Log::LogLevel::Info, logsPath);
	STMlog stmLog(logsPath);
	log.info(__HEADER__ "Start of initialisation.");
	try{
		Factory factory(configPath + "/raspberryPlatform.bin", configPath + "/stmPlatform.bin");
		auto platform = factory.clearAndCreate();
		*platform->ledBlue = true;
		*platform->ledRed = true;
		*platform->ledYellow = true;
		*platform->ledGreen = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// platform = factory.clearAndCreate();
		*platform->ledBlue = false;
		*platform->ledRed = false;
		*platform->ledYellow = false;
		*platform->ledGreen = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		FinishAfterEnterButton wather;
		unsigned int cycle = 0;
		if(platform->getMode() == SpiInterfaceMode::SpiInterfaceMode_NormalMode){
			while(wather.checkFlag()){
				if(platform->getAndClearCycleFlag()){
					std::cout << "\r(" << std::setw(4) << cycle++ << ")"
					<< " X: " << std::setw(4) << platform->currentPosition.x
					<< " Y: " << std::setw(4) << platform->currentPosition.y
					<< " angle: " << std::setw(4) << platform->currentPosition.angle
					<< " battery1: " << std::setw(3) << platform->batteryCell[0]
					<< " battery2: " << std::setw(3) << platform->batteryCell[1]
					<< " distance1: " << std::setw(4) << int(platform->distance[0])
					<< " distance2: " << std::setw(4) << int(platform->distance[1])
					<< " distance3: " << std::setw(4) << int(platform->distance[2])
					<< std::flush;
					// std::cout << "\r(" << std::setw(4) << cycle++ << ")"
					// << " doubleValue: " << std::setw(5) << platform->getValuePID
					// << " battery1: " << std::setw(3) << platform->batteryCell[0]
					// << " battery2: " << std::setw(3) << platform->batteryCell[1]
					// << std::flush;
				}else{
					std::cout << "\r(" << std::setw(4) << cycle++
						<< ")        STM not ready.        " << std::flush;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(40));
			}
		}else{
			std::cout << "Set percent of engines power" << std::endl;
			double percent = 0.0;
			std::cin >> percent;
			percent = std::min(percent, 100.0);
			percent = std::max(percent, -100.0);
			platform->setValuePID = percent;
			std::cout << "Engines power = " << platform->setValuePID << "%" << std::endl;
			std::cin.get();
			platform->setValuePID = 0.0;
			std::cout << "Engines power = " << platform->setValuePID << "%" << std::endl;
			std::cin.get();
		}
	}catch(...){
		log.error(__HEADER__ "Exception catched.");
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	Drivers::makeGPIO(0, false);
	return 0;
}
