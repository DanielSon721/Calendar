#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

static int comp_name(const void *ptr1, const void *ptr2);

typedef struct task_info {
  double cost;
  char *prog_language;
} Task_info;

static int comp_name(const void *ptr1, const void *ptr2) {
   return strcmp(((Event *)ptr1)->name, ((Event *)ptr2)->name);
}

static Task_info *create_task_info(double cost, const char *prog_language) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->prog_language = malloc(strlen(prog_language) + 1);
      if (task_info->prog_language) {
         task_info->cost = cost;
         strcpy(task_info->prog_language, prog_language);
         return task_info;
      }
   }

   return NULL;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->prog_language);
   free(task_info);
}

int main(void) {
   int days = 8, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_name, free_task_info, &calendar);

   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes, 
             info, activity_day);

   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1); 

   print_calendar(calendar, stdout, 1);

   info = get_event_info(calendar, "debugging"); 
   printf("Debugging event info\n");
   printf("Cost: %.2f, Prog language: %s\n", info->cost, info->prog_language);

   destroy_calendar(calendar);

   exit(EXIT_SUCCESS);
}
