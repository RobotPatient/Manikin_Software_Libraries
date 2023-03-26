#ifndef SPI_BASE_HPP
#define SPI_BASE_HPP
namespace hal::spi{

typedef struct{
int **data;
int numOfElements;
}SpiSlaveData;

class SPISlave{
public:
    explicit SPISlave(SpiSlaveData* slave_data){}
    virtual void begin() = 0;
    virtual void deinit() = 0;

};
}
#endif