/*
 * gpio.h
 *
 *  Created on: 21.04.2013
 *      Author: igor
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <string>
#include <stdexcept>

class GPIOError : public std::runtime_error
{
public:
	explicit GPIOError(const std::string& text_err) : std::runtime_error(text_err) {}
	virtual ~GPIOError() throw() {}
};

class GPIO
{
public:
	enum Dir { IN, OUT };
	enum Level { LOW, HIGH};
	explicit GPIO(unsigned num);
	GPIO(unsigned num, Dir dir);
	GPIO(unsigned num, Dir dir, Level value);
	GPIO(unsigned num, Dir dir, Level value, Level not_active);
	virtual ~GPIO() throw();
	virtual void setdir(Dir dir);
	virtual Dir getdir() const;
	virtual void set(Level value);
	virtual Level get() const;
	virtual void set_not_active(Level not_active);
	virtual Level get_not_active() const;
private:
	void gpio_export();
	void gpio_unexport() throw();
	void gpio_set_direction(Dir dir);
	void gpio_set(Level value);
	void gpio_set_no_throw(Level value) throw();
	Level gpio_get() const;
	unsigned num_;
	Level not_active_;
};



#endif /* GPIO_H_ */
