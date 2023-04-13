#ifndef SPI_BASE_HPP
#define SPI_BASE_HPP
#include <stdint.h>
namespace hal::spi{

typedef struct {
    uint8_t *data;
    uint8_t size;
} SpiSlaveData;



class SPISlave{
public:
    explicit SPISlave(){}
    virtual void begin() = 0;
    virtual uint32_t pollread() = 0;
    virtual void pollwrite(uint32_t data) = 0;
    virtual void deinit() = 0;
};
}
#endif