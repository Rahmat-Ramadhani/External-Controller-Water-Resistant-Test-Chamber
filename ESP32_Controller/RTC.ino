//#include "uRTCLib.h"

uRTCLib rtc(0x68);

// Function to get formatted date string
String numberFormatted(int number) {
  if(number < 10){
    return "0" + String(number);
  }else{
    return String(number);
  }
}

void adjustTime(int &secondSet, int &minuteSet, int &hourSet, int &dayOfWeekSet, int &dayOfMonthSet, int &monthSet, int &yearSet) {
  rtc.set(secondSet, minuteSet, hourSet, dayOfWeekSet, dayOfMonthSet, monthSet, yearSet);
}

String getFormattedDate() {
  rtc.refresh();
  return "20" + String(rtc.year()) + "-" + numberFormatted(rtc.month()) + "-" + numberFormatted(rtc.day());
}

// Function to get formatted time string
String getFormattedTime() {
  rtc.refresh();
  return numberFormatted(rtc.hour()) + ":" + numberFormatted(rtc.minute()) + ":" + numberFormatted(rtc.second()) + " WIB";
}

String getFormattedTimeHourAndMinute() {
  rtc.refresh();
  return numberFormatted(rtc.hour()) + ":" + numberFormatted(rtc.minute()) + " WIB";
}
