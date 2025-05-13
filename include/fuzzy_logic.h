#pragma once
#include "fuzzy_utils.h"
#include "esphome.h"


// funzione helper che spacchetta le soglie
std::array<float, 4> getTempTrapezoidThresholds(const std::array<float, 6>& soglie) {
    return { soglie[0], soglie[1], soglie[2], soglie[3] };
}


// funzione helper che spacchetta le soglie
std::array<float, 2> getTempRShoulderThresholds(const std::array<float, 6>& soglie) {
    return { soglie[4], soglie[5] };
}


// -------------------------------------------------------------------------
// membership


float temperatura_bassa(float temperatura, const std::array<float, 6>& soglie) {
    auto thres = getTempTrapezoidThresholds(soglie);
    return trapezoid(temperatura, thres[0], thres[1], thres[2], thres[3]);
}


float temperatura_alta(float temperatura, const std::array<float, 6>& soglie) {
    auto thres = getTempRShoulderThresholds(soglie);
    return right_shoulder(temperatura, thres[0], thres[1]);
}