#include <inp_master.hpp>

void IntraNetProtocol_master::initialize() {
    driver_->Init();
}

void IntraNetProtocol_master::
  inp_sensor_reading(slave_address_t hub_address, slave_reg_addr_t reg, sensor_type_id_t sen, uint8_t* data) {
    driver_->register_write(hub_address, reg, sen);
    driver_->register_read(hub_address, 1, data);
}

