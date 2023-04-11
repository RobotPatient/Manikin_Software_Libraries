#ifndef SPI_MAINBOARD_HPP
#define SPI_MAINBOARD_HPP
#include <spi_base.hpp>


inline constexpr 
uint8_t STATUS_REG_SIZE = 4;

inline constexpr 
uint8_t BBSET_REG_SIZE = 4;

inline constexpr 
uint8_t REQWORDS_REG_SIZE = 3;

inline constexpr 
uint8_t NUM_OF_BACKBONES = 4;

extern uint8_t STATUS;
extern uint8_t REQWORDS[NUM_OF_BACKBONES][REQWORDS_REG_SIZE];
extern uint8_t *SENSORDATA;
extern uint8_t *ACTDATA;
extern uint8_t BBSET[NUM_OF_BACKBONES][BBSET_REG_SIZE];
extern uint8_t SENSORDATA_size;
extern uint8_t ACTDATA_size;
void read_bbset();
namespace hal::spi{

class SPIMainBoard : public SPISlave{
    public:
        explicit  SPIMainBoard(SpiSlaveData* slave_data) : SPISlave(slave_data){
            data_ = slave_data;
        }
        void begin();
        uint32_t pollread();
        void pollwrite(uint32_t data);
        void deinit();
    private:
    SpiSlaveData* data_;
};
}
#endif