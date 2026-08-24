#pragma once

#include "./common.h"

enum class InternalSignal {
    Increment,
    Decrement,
    AskIsFull,
    NotFull,
    Full
};

enum Action {
    RespondToQuery,
    ReportOccupancy
};
