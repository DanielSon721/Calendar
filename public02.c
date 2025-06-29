#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

static int comp_duration_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

int main(void) {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   init_calendar("Summer Calendar", days, comp_duration_minutes, NULL, &calendar);

   add_event(calendar, "zoo visit", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "review meeting", 800, 80, NULL, 1); 
   add_event(calendar, "group meeting", 500, 60, NULL, 1);
   add_event(calendar, "lunch", 1200, 45, NULL, 3);
   
   print_calendar(calendar, stdout, 1);

   destroy_calendar(calendar);

   /****** gathering memory checking info *****/
   stop_memory_check();
   /****** gathering memory checking info *****/

   exit(EXIT_SUCCESS);
}

