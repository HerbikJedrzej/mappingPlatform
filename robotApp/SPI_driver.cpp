#include <SPI_driver.hh>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <logSystem.hh>

namespace Drivers{

class SPI : public SPIifc {
	Log log;
	const int frequency;
	const std::string name;
	const uint8_t bits;
	int fd;
	struct spi_ioc_transfer spi;
public:
  /**
   * Init's the SPI Connection
   * @method init
   * --------------------
   * @param  device    the Path to the SPI file
   * @param  mode      spi mode
   * @param  bits      bits per word
   * @param  LSB       lsb or msb
   * @param  speed     speed in herz
   * @param  nameParam human readable name of spi conn
   * @// return           file descriptor if successfull
   */
	SPI(const std::string& device,
        uint8_t     mode,
        uint8_t     bitsParam,
        uint8_t     LSB,
        uint32_t    speed,
        const std::string& nameParam):
	frequency(speed),
	name(nameParam),
	bits(bitsParam),
	fd(open(device.c_str(), O_RDWR))
	{
		if (fd< 0)
			throw std::invalid_argument("Error Open SPI Device");
		if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
			throw std::invalid_argument("Error Set SPI-Mode");
		if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0)
			throw std::invalid_argument("Error Get SPI-Mode");
		if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &LSB) < 0)
			throw std::invalid_argument("Error Set SPI LSB-Mode");
		if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
			throw std::invalid_argument("Error Set SPI wordlength");
		if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
			throw std::invalid_argument("Error Get SPI wordlength");
		if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
			throw std::invalid_argument("Error Set SPI Speed");
		if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
			throw std::invalid_argument("Error Get SPI Speed");
		log.info(__HEADER__ "SPI-Connection.: " + name);
		log.info(__HEADER__ "SPI-Device.....: " + device);
		log.info(__HEADER__ "SPI-Mode.......: " + std::to_string(mode));
		log.info(__HEADER__ "Wordlength.....: " + std::to_string(bits));
		log.info(__HEADER__ "Speed..........: "  + std::to_string(speed) + " Hz");
		log.info(__HEADER__ "Filehandle.....: "  + std::to_string(fd));
	}

	~SPI(){
		if(fd > 0) {
			close(fd);
		}
	};

	bool transfer(uint8_t* dataSend, uint8_t* dataRecive, const unsigned int& length) override{
		memset(&spi, 0, sizeof(spi));
		spi.tx_buf        = (unsigned long)dataSend;
		spi.rx_buf        = (unsigned long)dataRecive;
		spi.len           = length;
		spi.delay_usecs   = 0;
		spi.speed_hz      = frequency;
		spi.bits_per_word = bits;
		spi.cs_change     = 0;
		int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &spi);
		if (ret >= 0)
			return true;
		if(errno == EBADF)
			log.error(__HEADER__ "Fd is not a valid file descriptor.");
		if(errno == EFAULT)
			log.error(__HEADER__ "Argp references an inaccessible memory area");
		if(errno == EINVAL)
			log.error(__HEADER__ "Request or argp is not valid");
		if(errno == ENOTTY)
			log.error(__HEADER__ "Fd is not associated with a character special device");
		return false;
	}
};

std::shared_ptr<SPIifc> makeSPI(const std::string& device){
    std::shared_ptr<SPIifc> toReturn(new SPI(device, !SPI_CPOL | !SPI_CPHA, 8, 0, 16000, "MX11060"));
	return toReturn;
}

}
