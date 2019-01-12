#pragma once
class Passanger
{
public:
	unsigned long direction;
	unsigned long bus_stop_label;
	int come_time;
	int bus_enter_time;
	int arrived_time;

	Passanger(void);
	~Passanger(void);
};

