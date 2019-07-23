/*
 * ds1b820.h
 *
 * Created: 2019-07-23 오후 10:15:16
 *  Author: kijun
 */ 


#ifndef DS1B820_H_
#define DS1B820_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DQ_INPUT  (DDRA &= ~(1 << 7))
#define DQ_OUTPUT (DDRA |= (1 << 7))
#define DQ_HIGH   (PORTA |= (1 << 7))
#define DQ_LOW    (PORTA &= ~(1 << 7))
#define DQ        (PINA & (1 << 7) == 0x00) /* 0: high, 1: low */



#endif /* DS1B820_H_ */