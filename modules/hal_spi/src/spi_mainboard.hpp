#ifndef SPI_MAINBOARD_HPP
#define SPI_MAINBOARD_HPP

#include <spi_base.hpp>


inline constexpr
        uint8_t
STATUS_REG_SIZE = 4;

inline constexpr
        uint8_t
BBSET_REG_SIZE = 3;

inline constexpr
        uint8_t
REQWORDS_REG_SIZE = 3;

inline constexpr
        uint8_t
NUM_OF_BACKBONES = 4;

void read_bbset();

namespace hal::spi {


    class SPIMainBoard : public SPISlave {
    public:
        explicit SPIMainBoard(SpiSlaveData *slave_data) : SPISlave(slave_data) {
            slave_data = data;
        }

        void begin();

        uint32_t pollread();

        void pollwrite(uint32_t data);

        void deinit();

        static SpiSlaveData data[11];
    private:

    };
}
#endif