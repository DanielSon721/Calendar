#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"
#include "event.h"

int init_calendar(const char *name, int days, int (*compare) (const void *, const void *), void (*free) (void *), Calendar **calendar) {

    /*checks for invalid parameters*/
    if (days < 1 || name == NULL || calendar == NULL) {
        return FAILURE;
    }

    /*allocates memory calendar*/
    *calendar = (Calendar *)malloc(sizeof(Calendar));
    if (*calendar == NULL) {
        return FAILURE;  /*memory allocation failure*/
    }

    /*allocates memory and initializes name*/
    (*calendar)->name = (char *)malloc(strlen(name) + 1);
    if ((*calendar)->name == NULL) {
        free(*calendar);
        return FAILURE;
    }
    strcpy((*calendar)->name, name);

    /*allocates memory for events*/
    (*calendar)->events = (Event**)calloc(days, sizeof(Event *));
    if ((*calendar)->events == NULL) {
        free((*calendar)->name);
        free(*calendar);
        return FAILURE;
    }

    /*initializes other attributes*/
    (*calendar)->days = days;
    (*calendar)->total_events = 0;
    (*calendar)->comp_func = compare;
    (*calendar)->free_info_func = free;

    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *file, int extra_info) {

    int i;
    Event *curr;

    /*checks for invalid parameters*/
    if (calendar == NULL || file == NULL) {
        return FAILURE;
    }

    if (extra_info) {
        fprintf(file, "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n\n", calendar->name, calendar->days, calendar->total_events);
    }
    
    fprintf(file, "**** Events ****\n");

    /*pints info for each day*/
    for (i = 0; i < calendar->days; i++) {

        if (calendar->total_events != 0) {
            printf("Day %d\n", i + 1);
        }

        curr = calendar->events[i];

        /*prints all events in day*/
        while (curr != NULL) {
            fprintf(file, "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", curr->name, curr->start_time, curr->duration_minutes);
            curr = curr->next;
        }
    }

    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time, int duration, void *info, int day) {

    Event *curr, *prev, *new_event;

    /*checks for invalid parameters*/
    if (calendar == NULL || name == NULL || day < 1 || day > calendar->days || start_time < 0 || start_time > 2400 || duration < 1) {
        return FAILURE;
    }

    /*allocates memory for event*/
    new_event = (Event *)malloc(sizeof(Event));
    if (new_event == NULL) {
        return FAILURE;
    }

    /*allocates memory and initializes name*/
    new_event->name = (char *)malloc(strlen(name) + 1);
    if (new_event->name == NULL) {
        free(new_event);
        return FAILURE;
    }
    strcpy(new_event->name, name);

    /*initializes other attributes*/
    new_event->start_time = start_time;
    new_event->duration_minutes = duration;
    new_event->info = info;

    curr = calendar->events[day - 1];
    prev = NULL;

    /*loops to position of insertion*/
    while (curr != NULL) {

        /*stops loop where event will be inserted in sorted order*/
        if (calendar->comp_func(new_event, curr) < 0) {
            break;
        }
        /*event already exists*/
        else if (strcmp(curr->name, name) == 0) {
            return FAILURE;
        }
        
        prev = curr;
        curr = curr->next;
    }

    /*rearranges nodes*/
    if (prev == NULL) {
        new_event->next = calendar->events[day - 1];
        calendar->events[day - 1] = new_event;
    }
    else {
        prev->next = new_event;
        new_event->next = curr;
    }

    calendar->total_events++;

    return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **output) {

    int i;

    /*checks for invalid parameters*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }

    /*loops through each day and checks for event*/
    for (i = 0; i < calendar->days; i++) {
        if (find_event_in_day(calendar, name, i + 1, output) == SUCCESS) {
            return SUCCESS;
        }
    }

    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **output) {
    
    Event *curr;

    /*checks for invalid parameters*/
    if (calendar == NULL || name == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    curr = calendar->events[day - 1];

    /*loos through all events in day*/
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            *output = curr;
            return SUCCESS;
        }
        curr = curr->next;
    }

    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {

    int i;
    Event *curr, *prev;

    /*checks for invalid parameters*/
    if (calendar == NULL || name == NULL) {
         return FAILURE;
    }

    /*loops through all days*/
    for (i = 0; i < calendar->days; i++) {

        curr = calendar->events[i];
        prev = NULL;
        
        /*loops through all events in day*/
        while (curr != NULL) {
            if (strcmp(curr->name, name) == 0) {

                /*rearranges nodes*/
                if (prev == NULL) {
                    calendar->events[i] = curr->next;
                }
                else {
                    prev->next = curr->next;
                }
                
                /*frees memory*/
                if (curr->info != NULL && calendar->free_info_func != NULL) {
                    calendar->free_info_func(curr->info);
                }
                free(curr->name);
                free(curr);
                
                calendar->total_events--;

                return SUCCESS;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {

    int i;
    Event *curr;

    /*loops through all days*/
    for (i = 0; i < calendar->days; i++) {

        curr = calendar->events[i];

        /*loops through all events in day*/
        while (curr != NULL) {
            /*if match is found*/
            if (strcmp(curr->name, name) == 0) {
                return curr->info;
            }
            curr = curr->next;
        }
    }

    return NULL;
}

int clear_day(Calendar *calendar, int day) {
 
    Event *curr = calendar->events[day - 1], *next;
    
    /*checks for invalid parameters*/
    if (calendar == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }
    
    /*loops through all events in day*/
    while (curr != NULL) {

        next = curr->next;
        /*frees memory*/
        if (curr->info != NULL && calendar->free_info_func != NULL) {
            calendar->free_info_func(curr->info);
        }
        free(curr->name);
        free(curr);

        calendar->total_events--;
        curr = next;
    }

    calendar->events[day - 1] = NULL;

    return SUCCESS;
}

int clear_calendar(Calendar *calendar) {

    int i;

    /*checks for invalid parameters*/
    if (calendar == NULL) {
        return FAILURE;
    }

    /*loops through each day and clears events*/
    for (i = 0; i < calendar->days; i++) {
        clear_day(calendar, i + 1);
    }

    return SUCCESS;
}

int destroy_calendar(Calendar *calendar) {

    int i;
    Event *curr, *next;

    /*checks for invalid parameters*/
    if (calendar == NULL) {
        return FAILURE;
    }

    /*frees all memory*/
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar->name);
    free(calendar);

    return SUCCESS;
}