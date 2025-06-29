#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

static int comp_name(const void *ptr1, const void *ptr2) {
   return strcmp(((Event *)ptr1)->name, ((Event *)ptr2)->name);
}

int main(void) {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   Event *event;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   init_calendar("Summer Calendar", days, comp_name, NULL, &calendar);

   add_event(calendar, "zoo visit", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "review meeting", 800, 80, NULL, 1); 
   add_event(calendar, "group meeting", 500, 60, NULL, 1);
   add_event(calendar, "lunch", 1200, 45, NULL, 3);
   
   if (find_event(calendar, "review meeting", &event) == SUCCESS) {
      printf("Event found: %s, %d, %d\n", event->name, event->start_time, 
 	    event->duration_minutes);
   } else {
      printf("event \"review meeting\" not found\n");
   }

   if (find_event(calendar, "weekend meeting", &event) == SUCCESS) {
      printf("%s, %d, %d\n", event->name, event->start_time, event->duration_minutes);
   } else {
      printf("event \"weekend meeting\" not found\n");
   }

   destroy_calendar(calendar);

   /****** gathering memory checking info *****/
   stop_memory_check();
   /****** gathering memory checking info *****/

   exit(EXIT_SUCCESS);
}
