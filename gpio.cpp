/*
 * gpio.cpp
 *
 *  Created on: 21.04.2013
 *      Author: igor
 */

#include "gpio.h"
#include <fstream>
#include <sstream>

GPIO::GPIO(unsigned num) : num_(num), not_active_(HIGH)
{
	gpio_export();
}

GPIO::GPIO(unsigned num, Dir dir) : num_(num), not_active_(HIGH)
{
	gpio_export();
	gpio_set_direction(dir);
}

GPIO::GPIO(unsigned num, Dir dir, Level value) : num_(num), not_active_(HIGH)
{
	gpio_export();
	gpio_set_direction(dir);
	gpio_set(value);
}

GPIO::GPIO(unsigned num, Dir dir, Level value, Level not_active) : num_(num), not_active_(not_active)
{
	gpio_export();
	gpio_set_direction(dir);
	gpio_set(value);
}

GPIO::~GPIO() throw()
{
	gpio_set_no_throw(not_active_);
	gpio_unexport();
}

void GPIO::setdir(Dir dir)
{
	gpio_set_direction(dir);
}

GPIO::Dir GPIO::getdir() const
{
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << num_ << "/direction";
	std::ifstream gpiodirection(oss.str().c_str());
	if (!gpiodirection)
	{
		throw GPIOError(std::string("Not open ") + oss.str());
	}
	std::string dir;
	gpiodirection >> dir;
	if (gpiodirection.bad() || gpiodirection.fail())
	{
		throw GPIOError(std::string("Error reading ") + oss.str());
	}
	if (dir == "out")
	{
		return OUT;
	}
	else if (dir == "in")
	{
		return IN;
	}
	else
	{
		throw GPIOError(std::string("Wrong value direction in ") + oss.str());
	}
}

void GPIO::set(Level value)
{
	gpio_set(value);
}

GPIO::Level GPIO::get() const
{
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << num_ << "/value";
	std::ifstream gpiovalue(oss.str().c_str());
	if (!gpiovalue)
	{
		throw GPIOError(std::string("Not open ") + oss.str());
	}
	unsigned value;
	gpiovalue >> value;
	if (gpiovalue.bad() || gpiovalue.fail())
	{
		throw GPIOError(std::string("Error reading ") + oss.str());
	}
	return Level(value);
}

void GPIO::gpio_export()
{
	std::ofstream gpioexport("/sys/class/gpio/export");
	if (!gpioexport)
	{
		throw GPIOError("Not open /sys/class/gpio/export");
	}
	gpioexport << num_;
	if (gpioexport.bad() || gpioexport.fail())
	{
		throw GPIOError("Error writing /sys/class/gpio/export");
	}
}

void GPIO::gpio_set_direction(Dir dir)
{
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << num_ << "/direction";
	std::ofstream gpiodirection(oss.str().c_str());
	if (!gpiodirection)
	{
		throw GPIOError(std::string("Not open ") + oss.str());
	}
	if (dir == OUT)
	{
		gpiodirection << "out";
	}
	else if (dir == IN)
	{
		gpiodirection << "in";
	}
	else
	{
		throw GPIOError("Wrong direction");
	}
	if (gpiodirection.bad() || gpiodirection.fail())
	{
		throw GPIOError(std::string("Error writing ") + oss.str());
	}
}

void GPIO::gpio_set(Level value)
{
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << num_ << "/value";
	std::ofstream gpiovalue(oss.str().c_str());
	if (!gpiovalue)
	{
		throw GPIOError(std::string("Not open ") + oss.str());
	}
	if (value == LOW)
	{
		gpiovalue << 0;
	}
	else if (value == HIGH)
	{
		gpiovalue << 1;
	}
	else
	{
		throw GPIOError("Wrong level");
	}
	if (gpiovalue.bad() || gpiovalue.fail())
	{
		throw GPIOError(std::string("Error writing ") + oss.str());
	}
}

void GPIO::gpio_set_no_throw(Level value) throw()
{
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << num_ << "/value";
	std::ofstream gpiovalue(oss.str().c_str());
	if (value == LOW)
	{
		gpiovalue << 0;
	}
	else
	{
		gpiovalue << 1;
	}
}

void GPIO::gpio_unexport() throw()
{
	std::ofstream gpiounexport("/sys/class/gpio/unexport");
	gpiounexport << num_;
}

void GPIO::set_not_active(Level not_active)
{
	not_active_ = not_active;
}

GPIO::Level GPIO::get_not_active() const
{
	return not_active_;
}
