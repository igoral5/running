/*
 * main.cpp
 *
 *  Created on: 21.04.2013
 *      Author: igor
 */

#include <iostream>
#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <tr1/memory>
#include "gpio.h"

volatile bool work = true;

void
sig_handler(int signo)
{
	work = false;
}

int
main(int argc, char *argv[])
try
{
	struct sigaction act;
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		throw std::runtime_error("Error set signal handler");
	}
	std::vector<std::tr1::shared_ptr<GPIO> > leds;
	leds.push_back(std::tr1::shared_ptr<GPIO>(new GPIO(17, GPIO::OUT, GPIO::LOW, GPIO::HIGH)));
	leds.push_back(std::tr1::shared_ptr<GPIO>(new GPIO(18, GPIO::OUT, GPIO::HIGH, GPIO::HIGH)));
	leds.push_back(std::tr1::shared_ptr<GPIO>(new GPIO(27, GPIO::OUT, GPIO::HIGH, GPIO::HIGH)));
	leds.push_back(std::tr1::shared_ptr<GPIO>(new GPIO(22, GPIO::OUT, GPIO::HIGH, GPIO::HIGH)));
	leds.push_back(std::tr1::shared_ptr<GPIO>(new GPIO(23, GPIO::OUT, GPIO::HIGH, GPIO::HIGH)));
	GPIO button(24, GPIO::IN);
	GPIO::Level old_button_stat = GPIO::HIGH;
	std::vector<std::tr1::shared_ptr<GPIO> >::iterator led_on = leds.begin();
	while (work)
	{
		GPIO::Level new_button_stat = button.get();
		if (old_button_stat == GPIO::HIGH && new_button_stat == GPIO::LOW)
		{
			(*led_on)->set(GPIO::HIGH);
			led_on++;
			if (led_on == leds.end())
			{
				led_on = leds.begin();
			}
			(*led_on)->set(GPIO::LOW);
		}
		old_button_stat = new_button_stat;
		usleep(100000);
	}
	return EXIT_SUCCESS;
}
catch(const std::exception& er)
{
	std::cerr << argv[0] << " : " << er.what() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << argv[0] << " : " << "Unknown error" << std::endl;
	return EXIT_FAILURE;
}



