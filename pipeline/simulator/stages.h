#ifndef STAGES_H
#define STAGES_H

#include "global.h"
#include "define.h"
#include "check.h"

#include <string>

using namespace std;

void IF();
void IF2ID();

void ID();

void Control();

//void _move(string currentStage);
void ID2EX();

void EX();
void EX2DM();

void DM();
void DM2WB();

void WB();

#endif
