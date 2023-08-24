#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *) ptr1)->duration_minutes -
      ((Event *) ptr2)->duration_minutes;
}

/* Checks if upon initialization total events is 0 and destroy calendar
 function */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (!calendar->total_events) {
         if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }

   return FAILURE;
}

/* Checks add same name event on same day */
static int test2() {
   Calendar *calendar;
   if (!init_calendar("School Calendar", 5, comp_minutes, NULL, &calendar)) {
      if (!add_event(calendar, "study 216", 300, 60, NULL, 1)) {
         if (add_event(calendar, "study 216", 400, 55, NULL, 1)) {
            destroy_calendar(calendar);
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/* Checks add same name event on diff day */
static int test3() {
   Calendar *calendar;
   if (!init_calendar("School Calendar", 5, comp_minutes, NULL, &calendar)) {
      if (!add_event(calendar, "study 216", 300, 60, NULL, 1)) {
         if (add_event(calendar, "study 216", 400, 55, NULL, 4)) {
            destroy_calendar(calendar);
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/* checks init_calendar with null passed in calendar param, with name passed
 in as null, and with days less than 1 */
static int test4() {

   Calendar *calendar;
   if (init_calendar("Griddy", 1, comp_minutes, NULL, NULL)) {
      if (init_calendar(NULL, 1, comp_minutes, NULL, &calendar)) {
         if (init_calendar("Griddy", 0, comp_minutes, NULL, &calendar)) {
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/* Checks if null calendar or null output stream is passed into 
print_document*/
static int test5() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(NULL, stdout, 1)) {
         if (print_calendar(calendar, NULL, 1)) {
            destroy_calendar(calendar);
            return SUCCESS;
         }
      }
   }

   return FAILURE;
}

/* when compare == 0, new event goes before existing & adding event with no
 prexisting event and with an existing event */
static int test6() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 400, 60, NULL, 1);
   add_event(calendar, "study 250", 400, 60, NULL, 1);
   if (!strcmp(calendar->events[0]->name, "study 250")) {
      destroy_calendar(calendar);
      return SUCCESS;
   }

   return FAILURE;
}

/* add event at end, add head of list with existing events, add with start
 times 0 or 2400, add times with start times less than 0 and greater then 
2400, trying to add with null calendar or with null name, adding with 0 as
 duration minutes, adding when day is less than 1 and also print calendar with
 no extra info */
static int test7() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 1);
   add_event(calendar, "study 250", 2400, 60, NULL, 1);
   add_event(calendar, "study 330", 400, 120, NULL, 1);
   add_event(calendar, "study 132", 400, 15, NULL, 1);
   add_event(calendar, NULL, 400, 15, NULL, 1);
   add_event(NULL, "study 401", 400, 15, NULL, 1);
   add_event(calendar, "study 351", 400, 0, NULL, 1);
   add_event(calendar, "study 131", 400, 10, NULL, 0);
   add_event(calendar, "study 440", -1, 10, NULL, 1);
   add_event(calendar, "study 400", 2500, 10, NULL, 1);
   print_calendar(calendar, stdout, 0);
   if (!strcmp(calendar->events[0]->next->next->next->name, "study 330")) {
      destroy_calendar(calendar);
      return SUCCESS;
   }

   return FAILURE;
}

/* find event at head, tail and middle of the list as well as the out event
 parameter */
static int test8() {
   int days = 5;
   Calendar *calendar;
   Event *head, *middle, *tail;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 1);
   add_event(calendar, "study 250", 2400, 60, NULL, 1);
   add_event(calendar, "study 330", 400, 120, NULL, 1);
   find_event(calendar, "study 330", &tail);
   find_event(calendar, "study 216", &middle);
   find_event(calendar, "study 250", &head);
   if (!strcmp(tail->name, "study 330") && !strcmp(head->name, "study 250")
       && !strcmp(middle->name, "study 216")) {
      destroy_calendar(calendar);
      return SUCCESS;
   }
   return FAILURE;

}

/* tests find event if there are no events and if it will fill the event out
 parameter, if there are events but the event to find does not exist, if
 calendar or name is null */
static int test9() {
   int days = 5;
   Event *empty = NULL;
   Calendar *calendar;
   init_calendar("School", days, comp_minutes, NULL, &calendar);
   if (find_event(calendar, "study", &empty)) {
      add_event(calendar, "study 216", 000, 60, NULL, 1);
      add_event(calendar, "study 250", 2400, 60, NULL, 1);
      add_event(calendar, "study 330", 400, 120, NULL, 1);
      if (find_event(calendar, "study 132", NULL) && !empty) {
         if (find_event(NULL, "study 216", NULL)) {
            if (find_event(calendar, NULL, NULL)) {
               destroy_calendar(calendar);
               return SUCCESS;
            }
         }
      }
   }
   return FAILURE;
}

/* find event in day at head, tail and middle of the list as well as the out
 event parameter */
static int test10() {
   int days = 5;
   Calendar *calendar;
   Event *head, *middle, *tail;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 1);
   add_event(calendar, "study 250", 2400, 60, NULL, 1);
   add_event(calendar, "study 330", 400, 120, NULL, 1);
   find_event_in_day(calendar, "study 330", 1, &tail);
   find_event_in_day(calendar, "study 216", 1, &middle);
   find_event_in_day(calendar, "study 250", 1, &head);
   if (!strcmp(tail->name, "study 330") && !strcmp(head->name, "study 250")
       && !strcmp(middle->name, "study 216")) {
      destroy_calendar(calendar);
      return SUCCESS;
   }
   return FAILURE;

}

/* tests find event in day if there are no events and if it will fill the 
event out parameter, if there are events but the event to find does not exist,
 if calendar or name is null, if the event exists but on a different day, 
if the day surpasses the number of days in the calendar */
static int test11() {
   int days = 5;
   Event *empty = NULL;
   Calendar *calendar;
   init_calendar("School", days, comp_minutes, NULL, &calendar);
   if (find_event_in_day(calendar, "study", 1, &empty)) {
      add_event(calendar, "study 216", 000, 60, NULL, 1);
      add_event(calendar, "study 250", 2400, 60, NULL, 1);
      add_event(calendar, "study 330", 400, 120, NULL, 1);
      if (find_event_in_day(calendar, "study 132", 1, NULL) && !empty) {
         if (find_event_in_day(NULL, "study 216", 1, NULL)) {
            if (find_event_in_day(calendar, NULL, 1, NULL)) {
               if (find_event_in_day(calendar, "study 216", 2, NULL)) {
                  if (find_event_in_day(calendar, "study 216", 6, NULL))
                     destroy_calendar(calendar);
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

/* tests remove when event does not exist, when calendar or name is null,
 remove at the tail, remove at the head, remove at the middle, and remove 
event so that there are none left */
static int test12() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 2);
   add_event(calendar, "study 250", 2400, 60, NULL, 2);
   add_event(calendar, "study 330", 400, 120, NULL, 2);
   add_event(calendar, "study 132", 400, 15, NULL, 2);
   if (remove_event(calendar, "study") && remove_event(NULL, "study") &&
       remove_event(calendar, NULL)) {
      remove_event(calendar, "study 330");
      if (!strcmp(calendar->events[1]->next->next->name, "study 216")) {
         remove_event(calendar, "study 132");
         if (!strcmp(calendar->events[1]->next->name, "study 216")) {
            add_event(calendar, "study 132", 400, 15, NULL, 2);
            remove_event(calendar, "study 250");
            if (!strcmp(calendar->events[1]->next->name, "study 216")) {
               remove_event(calendar, "study 132");
               remove_event(calendar, "study 216");
               if (!calendar->events[1]) {
                  destroy_calendar(calendar);
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

/* tests get event info if the event is found and also if the event is not
 found */
static int test13() {
   int days = 5;
   Calendar *calendar;
   int *int_ptr = &days;
   void *ptr = NULL;
   void *pointer = &days;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, int_ptr, 2);
   ptr = get_event_info(calendar, "study 216");
   if ((int *) ptr == int_ptr) {
      pointer = get_event_info(calendar, "LOL");
      if (!pointer) {
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   return FAILURE;
}

/* tests clear_calendar on a calendar with no events, and with events in every
 single position, and also when calender param is null */
static int test14() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   if (!clear_calendar(calendar)) {
      add_event(calendar, "study 216", 000, 60, NULL, 1);
      add_event(calendar, "study 250", 2400, 60, NULL, 2);
      add_event(calendar, "study 330", 400, 120, NULL, 3);
      add_event(calendar, "study 132", 400, 15, NULL, 4);
      add_event(calendar, "study 440", 400, 15, NULL, 5);
      if (!clear_calendar(calendar)) {
         if (clear_calendar(NULL)) {
            destroy_calendar(calendar);
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/* clearing a day full of events while leaving one event in a different day,
 clearing day 0, when day is greater then number of days in calender, and 
clear day with null calendar */
static int test15() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 2);
   add_event(calendar, "study 250", 2400, 60, NULL, 2);
   add_event(calendar, "study 330", 400, 120, NULL, 2);
   add_event(calendar, "study 132", 400, 15, NULL, 2);
   add_event(calendar, "study ", 400, 15, NULL, 1);
   print_calendar(calendar, stdout, 1);
   if (!clear_day(calendar, 2)) {
      if (clear_day(calendar, 0) && clear_day(NULL, 1) &&
          clear_day(calendar, 6)) {
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   return FAILURE;
}

/* tests destroy calendar when there is an event every single day as well as
with null */
static int test16() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 1);
   add_event(calendar, "study 250", 2400, 60, NULL, 2);
   add_event(calendar, "study 330", 400, 120, NULL, 3);
   add_event(calendar, "study 132", 400, 15, NULL, 4);
   add_event(calendar, "study 440", 400, 15, NULL, 5);
   if (!destroy_calendar(calendar)) {
      if (destroy_calendar(NULL)) {
         return SUCCESS;
      }
   }
   return FAILURE;
}

/* calling clear day and then clear calender afterward */
static int test17() {
   int days = 5;
   Calendar *calendar;

   init_calendar("School", days, comp_minutes, NULL, &calendar);
   add_event(calendar, "study 216", 000, 60, NULL, 2);
   add_event(calendar, "study 250", 2400, 60, NULL, 2);
   add_event(calendar, "study 330", 400, 120, NULL, 2);
   add_event(calendar, "study 132", 400, 15, NULL, 2);
   add_event(calendar, "study ", 400, 15, NULL, 1);
   print_calendar(calendar, stdout, 1);
   if (!clear_day(calendar, 2)) {
      if (!clear_calendar(calendar)) {
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE)
      result = FAILURE;
   if (test2() == FAILURE)
      result = FAILURE;
   if (test3() == FAILURE)
      result = FAILURE;
   if (test4() == FAILURE)
      result = FAILURE;
   if (test5() == FAILURE)
      result = FAILURE;
   if (test6() == FAILURE)
      result = FAILURE;
   if (test7() == FAILURE)
      result = FAILURE;
   if (test8() == FAILURE)
      result = FAILURE;
   if (test9() == FAILURE)
      result = FAILURE;
   if (test10() == FAILURE)
      result = FAILURE;
   if (test11() == FAILURE)
      result = FAILURE;
   if (test12() == FAILURE)
      result = FAILURE;
   if (test13() == FAILURE)
      result = FAILURE;
   if (test14() == FAILURE)
      result = FAILURE;
   if (test15() == FAILURE)
      result = FAILURE;
   if (test16() == FAILURE)
      result = FAILURE;
   if (test17() == FAILURE)
      result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/

   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
