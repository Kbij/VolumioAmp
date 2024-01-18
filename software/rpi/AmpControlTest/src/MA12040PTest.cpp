/*
** File description:
** MA12040PTest
*/

#include "lib/MA12040P.h"
#include "Mock/GpioIfMock.h"
#include "Mock/I2CIfMock.h"
#include "gtest/gtest.h"

TEST(MA12040PTest, Constructor)
{
    HardwareNs::MA12040PSettings settings;
    HardwareNs::MA12040P* amplifier = new HardwareNs::MA12040P(settings, nullptr, nullptr);
    delete amplifier;
}

TEST(MA12040PTest, ErrorMonitor)
{
    auto gpio = new GpioIfMock;
    auto i2c = new I2CIfMock;
    HardwareNs::MA12040PSettings settings;
    HardwareNs::MA12040P* amplifier = new HardwareNs::MA12040P(settings, gpio, i2c);
    delete amplifier;
}
