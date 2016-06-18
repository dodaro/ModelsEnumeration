/*
*
*  Copyright 2016 Carmine Dodaro.
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*/

#include "glucose-syrup/core/Solver.h"
#include <climits>
#include <iostream>
using namespace Glucose;
using namespace std;

class EnumerationSolver : public Solver
{
private:
    unsigned int numberOfModels;
    unsigned int maxModels;
    vec<bool> checked;
    vec<int> inAssumptions;

    inline bool isAssumption(Lit l) const { return inAssumptions[var(l)]!=-1; }
    inline bool isChoice(Lit l) const { return reason(var(l)) == CRef_Undef; }
    inline void popAssumption() { inAssumptions[var(assumptions.last())]=-1; assumptions.pop(); }
    inline void pushAssumption(Lit l) { inAssumptions[var(l)]=assumptions.size(); assumptions.push(l); }
    inline bool resetAndCallSolver() { budgetOff(); return solve_() == l_True; }  //search for a model under assumptions
    int getBackjumpingLevel() const;
    bool foundModel();
    void flipLatestChoice();    

public:
    inline EnumerationSolver() : Solver(), numberOfModels(0), maxModels(UINT_MAX) { setIncrementalMode(); }
    void enumerate();
    inline void setMaxModels(unsigned int maxModels) { this->maxModels=maxModels; } //set the number of models to print
};
