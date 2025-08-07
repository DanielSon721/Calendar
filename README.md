# Calendar Event Management System

This project implements a dynamic calendar system in C that allows users to manage events across multiple days. It supports adding, removing, searching, printing, and clearing events with efficient memory handling and customizable behavior.

## Features

- Initialize a calendar with a custom name and number of days
- Add events to specific days in sorted order using a user-defined comparison function
- Remove individual events or clear entire days
- Search for events by name across all days
- Print calendar contents, optionally including metadata
- Access additional user-defined data (`info`) for each event
- Clean memory using custom deallocation logic

## File Structure

- `calendar.c` – Core implementation of the calendar and its functions
- `calendar.h` – Header file defining the `Calendar` structure and function prototypes
- `event.h` – Header file defining the `Event` structure

## Requirements

- GCC or any standard C compiler
- C standard library (`stdlib.h`, `string.h`, `stdio.h`)

## Compilation

```bash
gcc -o calendar main.c calendar.c
