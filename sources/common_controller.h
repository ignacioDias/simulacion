#pragma once

#include "./common.h"

// PCC SIGNALS
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
