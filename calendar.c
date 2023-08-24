#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"

static void clear_aux(Calendar * calendar, Event * head);

/* recursive aux method for clear */
static void clear_aux(Calendar * calendar, Event * head) {
   if (head != NULL) {
      clear_aux(calendar, head->next);
      remove_event(calendar, head->name);
   }
}

int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
   if (!calendar || days < 1 || !name) {
      return FAILURE;
   }
   *calendar = malloc(sizeof(Calendar));
   if (!(*calendar)) {
      return FAILURE;
   }
   (*calendar)->name = malloc(strlen(name) + 1);
   (*calendar)->events = calloc(days, sizeof(Event));
   if (!(*calendar)->name || !(*calendar)->events) {
      return FAILURE;
   }
   strcpy((*calendar)->name, name);
   (*calendar)->days = days;
   (*calendar)->total_events = 0;
   (*calendar)->comp_func = comp_func;
   (*calendar)->free_info_func = free_info_func;

   return SUCCESS;
}

int print_calendar(Calendar * calendar, FILE * output_stream, int print_all) {
   int i;
   Event *temp;

   if (!calendar || !output_stream) {
      return FAILURE;
   } else {
      if (print_all) {
         fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
         fprintf(output_stream, "Days: %d\n", calendar->days);
         fprintf(output_stream, "Total Events: %d\n\n",
                 calendar->total_events);
      }
      fprintf(output_stream, "**** Events ****\n");
      if (calendar->total_events > 0) {
         for (i = 0; i < calendar->days; i++) {
            fprintf(output_stream, "Day %d\n", i + 1);
            temp = calendar->events[i];
            while (temp) {
               fprintf(output_stream,
                       "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
                       temp->name, temp->start_time, temp->duration_minutes);
               temp = temp->next;
            }
         }
      }
      return SUCCESS;
   }
}

int add_event(Calendar * calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
   Event *temp;
   Event *curr, *prev, *store;

   if (!calendar || !name || start_time < 0 || start_time > 2400 ||
       duration_minutes <= 0 || day < 1 || day > calendar->days ||
       !find_event(calendar, name, NULL)) {
      return FAILURE;
   } else {
      temp = malloc(sizeof(Event));
      if (!temp) {
         return FAILURE;
      }
      temp->name = malloc(strlen(name) + 1);
      if (!temp->name) {
         return FAILURE;
      }
      strcpy(temp->name, name);
      temp->start_time = start_time;
      temp->duration_minutes = duration_minutes;
      temp->info = info;
      curr = calendar->events[day - 1];
      if (!curr || calendar->comp_func(temp, curr) <= 0) {      
      /* case: no event or belongs first event */
         prev = curr;
         calendar->events[day - 1] = temp;
         temp->next = prev;
      } else {
         while (curr != NULL && calendar->comp_func(temp, curr) >= 0) {
            prev = curr;
            curr = curr->next;
         }
         store = prev->next;
         prev->next = temp;
         temp->next = store;
      }
      calendar->total_events += 1;
      return SUCCESS;
   }
}

int find_event(Calendar * calendar, const char *name, Event ** event) {
   Event *curr;
   int i;

   if (!calendar || !name) {
      return FAILURE;
   } else {
      for (i = 0; i < calendar->days; i++) {
         curr = calendar->events[i];
         while (curr != NULL) {
            if (!strcmp(curr->name, name)) {
               if (event) {
                  *event = curr;
               }
               return SUCCESS;
            }
            curr = curr->next;
         }
      }
      return FAILURE;
   }
}

int find_event_in_day(Calendar * calendar, const char *name, int day,
                      Event ** event) {
   Event *curr;

   if (!calendar || !name || day < 1 || day > calendar->days) {
      return FAILURE;
   } else {
      curr = calendar->events[day - 1];
      while (curr != NULL) {
         if (!strcmp(curr->name, name)) {
            if (event) {
               *event = curr;
            }
            return SUCCESS;
         }
         curr = curr->next;
      }
      return FAILURE;
   }
}

int remove_event(Calendar * calendar, const char *name) {
   Event *remove, *curr, *new_next, *prev = NULL;
   int found, day, i;

   if (!calendar || !name) {
      return FAILURE;
   } else {
      found = find_event(calendar, name, &remove);
      if (found) {
         return FAILURE;
      } else {
         for (i = 0; i < calendar->days; i++) {
            if (!find_event_in_day(calendar, name, i + 1, NULL)) {
               day = i;
               break;
            }
         }
         curr = calendar->events[day];
         while (curr != NULL) {
            if (!strcmp(curr->name, remove->name)) {
               break;
            }
            prev = curr;
            curr = curr->next;
         }
         if (!prev && !strcmp(curr->name, remove->name)) {
            calendar->events[day] = remove->next;
         } else {
            new_next = curr->next;
            curr->next = NULL;
            prev->next = new_next;
         }
         if (calendar->free_info_func && remove->info != NULL) {
            calendar->free_info_func(remove->info);
         }
         free(remove->name);
         free(remove);
         calendar->total_events -= 1;
         return SUCCESS;
      }
   }
}

void *get_event_info(Calendar * calendar, const char *name) {
   int found;
   Event *for_info;

   found = find_event(calendar, name, &for_info);
   if (found) {
      return NULL;
   }
   return for_info->info;
}

int clear_calendar(Calendar * calendar) {
   int i;
   if (!calendar) {
      return FAILURE;
   } else {
      if (!calendar->total_events) {
         return SUCCESS;
      }
      for (i = 0; i < calendar->days; i++) {
         if (!calendar->events[i]) {
            continue;
         } else {
            clear_aux(calendar, calendar->events[i]);
         }
      }
      return SUCCESS;
   }
}

int clear_day(Calendar * calendar, int day) {
   Event *curr;
   int counter = 0;
   if (!calendar || day < 1 || calendar->days < day) {
      return FAILURE;
   } else {
      curr = calendar->events[day - 1];
      while (curr != NULL) {
         counter += 1;
         curr = curr->next;
      }
      clear_aux(calendar, calendar->events[day - 1]);
      return SUCCESS;
   }
}

int destroy_calendar(Calendar * calendar) {
   if (!calendar) {
      return FAILURE;
   } else {
      clear_calendar(calendar);
      free(calendar->events);
      free(calendar->name);
      free(calendar);
      return SUCCESS;
   }
}
