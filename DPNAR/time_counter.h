#pragma once
#include <time.h>

class time_counter
{
	/* su dung de tinh thoi gian thuc hien cua thuat toan.*/
private:
	time_t starting_time;
public:
	time_counter(void);
public:
	~time_counter(void);
	void settime();
	int running_period();
};
