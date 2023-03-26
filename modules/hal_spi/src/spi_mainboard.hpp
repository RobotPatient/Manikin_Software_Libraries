#ifndef SPI_MAINBOARD_HPP
#define SPI_MAINBOARD_HPP
#include <spi_base.hpp>

namespace hal::spi{

class SPIMainBoard : public SPISlave{
    public:
        explicit  SPIMainBoard(SpiSlaveData* slave_data) : SPISlave(slave_data){
            data_ = slave_data;
        }
        void begin();
        void deinit();
    private:
    SpiSlaveData* data_;
};
}
#endif