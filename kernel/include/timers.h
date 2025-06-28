/**
    timers.h

    timers utilities
**/

#pragma once

#include <efi.h>
#include <efilib.h>

#include "motor.h"

struct _time_modifier {
    u8                                  hr;
    u8                                  min;
    u8                                  sec;
};

struct _timer_struct {
    u64                                 duration; // second

    struct _time_modifier               DurationRt;

    EFI_TIME                            StartAt;

    EFI_TIME                            EndsAt; // edit in running time
};

struct
_timer_struct
InitTimer
(
        u64                             duration
)
{
    //
    // setuping
    //

    struct _timer_struct                timer;
    struct _time_modifier               mod;

    u8                                  minutes;
    u8                                  hours;
    u8                                  seconds;

    //
    // calculate
    //

    gRT->GetTime(&timer.StartAt,0);

    minutes =                           duration / 60;
    hours =                             minutes / 60;

    seconds =                           duration - (minutes * 60);

    mod.sec = seconds;
    mod.min = minutes;
    mod.hr = hours;

    //
    // setting
    //

    timer.DurationRt = mod;

    timer.EndsAt = timer.StartAt;

    timer.EndsAt.Second += timer.DurationRt.sec;
    timer.EndsAt.Minute += timer.DurationRt.min;
    timer.EndsAt.Hour += timer.DurationRt.hr;

    return timer;

}