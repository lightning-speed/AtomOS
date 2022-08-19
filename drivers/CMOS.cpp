#include <CMOS.h>
#include <IO.h>
namespace CMOS
{
	int getSeconds()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x0);
		int second = IO::inb(cmos_data);
		second = (second & 0x0F) + ((second / 16) * 10);
		return second;
	}
	int getMinutes()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x02);
		int minute = IO::inb(cmos_data);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		return minute;
	}
	int getHours()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x04);
		int hour = IO::inb(cmos_data);
		hour = (hour & 0x0F) + ((hour / 16) * 10);
		return hour;
	}

	int getDays()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x07);
		int days = IO::inb(cmos_data);
		days = (days & 0x0F) + ((days / 16) * 10);
		return days;
	}

	int getMonths()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x08);
		int months = IO::inb(cmos_data);
		months = (months & 0x0F) + ((months / 16) * 10);
		return months;
	}
	int getYears()
	{
		int cmos_address = 0x70;
		int cmos_data = 0x71;
		IO::outb(cmos_address, 0x09);
		int year = IO::inb(cmos_data);
		year = (year & 0x0F) + ((year / 16) * 10);
		return year;
	}
};
