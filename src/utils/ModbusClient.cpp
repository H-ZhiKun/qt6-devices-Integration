#include "ModbusClient.h"

ModbusClient::ModbusClient(const std::string &ipAddress, int port)
    : ipAddress(ipAddress), port(port), modbusContext(nullptr)
{
    modbusContext = modbus_new_tcp(ipAddress.c_str(), port);
    if (!modbusContext)
    {
        LOGERROR("Failed to create Modbus context");
    }

    if (modbus_connect(modbusContext) == -1)
    {
        LOGERROR("Failed to connect to Modbus device");
    }
}

ModbusClient::~ModbusClient()
{
    modbus_free(modbusContext);
}

std::vector<uint16_t> ModbusClient::readRegisters(uint16_t address, uint16_t count)
{
    checkConnection();

    std::lock_guard<std::mutex> lock(mutex);

    std::vector<uint16_t> values;
    values.resize(addresses.size());

    bool allCached = true;
    for (size_t i = 0; i < addresses.size(); ++i)
    {
        auto it = cache.find(addresses[i]);
        if (it == cache.end())
        {
            allCached = false;
            break;
        }
        values[i] = it->second;
    }

    if (!allCached)
    {
        if (modbus_read_registers(modbusContext, addresses[0], addresses.size(), values.data()) == -1)
        {
            reconnectOnError();
            LOGERROR("Failed to read registers");
        }

        updateCache(addresses, values);
    }

    return values;
}

void ModbusClient::writeRegisters(const std::vector<uint16_t> &addresses, const std::vector<uint16_t> &values)
{
    checkConnection();

    std::lock_guard<std::mutex> lock(mutex);

    if (modbus_write_registers(modbusContext, addresses[0], addresses.size(), values.data()) == -1)
    {
        reconnectOnError();
        LOGERROR("Failed to write registers");
    }

    updateCache(addresses, values);
}

void ModbusClient::updateCache(const std::vector<uint16_t> &addresses, const std::vector<uint16_t> &values)
{
    for (size_t i = 0; i < addresses.size(); ++i)
    {
        cache[addresses[i]] = values[i];
    }
}

void ModbusClient::checkConnection()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (modbus_get_socket(modbusContext) == -1)
    {
        reconnect();
    }
}

void ModbusClient::reconnect()
{
    modbus_free(modbusContext);
    modbusContext = nullptr;

    modbusContext = modbus_new_tcp(ipAddress.c_str(), port);
    if (!modbusContext)
    {
        LOGERROR("Failed to create Modbus context");
    }

    if (modbus_connect(modbusContext) == -1)
    {
        LOGERROR("Failed to connect to Modbus device");
    }
    cache.clear();
}

void ModbusClient::reconnectOnError()
{
    if (modbus_get_socket(modbusContext) == -1)
    {
        reconnect();
    }
}
