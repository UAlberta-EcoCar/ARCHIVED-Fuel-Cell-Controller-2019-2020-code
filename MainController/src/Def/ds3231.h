/*
 * File:   ds3231.h
 * Author: |H|e|c|t|o|r|
 *
 */

#ifndef DS3231_H
#define	DS3231_H

#define DS3231_ADDRESS 0b1101000

#define MODE_12HOUR_AM  0x40
#define MODE_12HOUR_PM  0X60
#define SECONDS         0x00
#define MINUTES         0x01
#define HOURS           0x02
#define DAY             0x03
#define DATE            0x04
#define MONTH           0x05
#define YEAR            0x06

#endif	/* DS3231_H */