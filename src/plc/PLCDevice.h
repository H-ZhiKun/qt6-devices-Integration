#pragma once
#include "RegistersWapper.h"
class PLCDevice
{
  public:
    explicit PLCDevice();
    ~PLCDevice();

  protected:
    PLCDevice(const PLCDevice &) = delete;
    PLCDevice &operator=(const PLCDevice &) = delete;
    PLCDevice(PLCDevice &&) noexcept(true) = default;
    PLCDevice &operator=(PLCDevice &&) noexcept(true) = default;

  private:
    RegistersWapper regWapper;
};