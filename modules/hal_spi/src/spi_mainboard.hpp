#ifndef SPI_MAINBOARD_HPP
#define SPI_MAINBOARD_HPP

#include <spi_base.hpp>

namespace hal::spi {
    inline constexpr
    uint8_t kSTATUS_REG = 0;

    inline constexpr
    uint8_t kBBSETA_REG = 0x01;

    inline constexpr
    uint8_t kBBSETB_REG = 0x02;

    inline constexpr
    uint8_t kBBSETC_REG = 0x03;

    inline constexpr
    uint8_t kBBSETD_REG = 0x04;

    inline constexpr
    uint8_t kREQWORDA_REG = 0x05;

    inline constexpr
    uint8_t kREQWORDB_REG = 0x06;

    inline constexpr
    uint8_t kREQWORDC_REG = 0x07;

    inline constexpr
    uint8_t kREQWORDD_REG = 0x08;

    inline constexpr
    uint8_t kSENSDATA_REG = 0x09;

    inline constexpr
    uint8_t kACTDATA_REG = 0x0A;

    extern SpiSlaveData SPIMainboard_reg_data_[11];

    class SPIMainBoard : public SPISlave {
    public:
        explicit SPIMainBoard() : SPISlave() {
        }

        void begin();

        uint32_t pollread();

        void pollwrite(uint32_t data);

        void deinit();
    private:

    };
}
#endif