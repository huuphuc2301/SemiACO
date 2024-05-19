/**
 * @author: Rafał Skinderowicz (rafal.skinderowicz@us.edu.pl)
 */
#pragma once

#include <string>

struct ProgramOptions {

    uint32_t ants_count_ = 40;

    int32_t iterations_ = 300;

    double rho_ = 0.05;
    double tau_max_ = 1.0;
    double tau_min_ ;

    int target_ = 10;

    double q0 = 0.3;
};