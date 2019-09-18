#ifndef LCD_MONITOR_H
#define LCD_MONITOR_H

void setupMonitor();
void updateTime(byte timeRemain);
void updateTime(byte pos, byte timeRemain);
void countdown();
void showMonitor(String s);

#endif
