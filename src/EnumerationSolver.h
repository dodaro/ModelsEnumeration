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
    vec<bool> inAssumptions;
    inline void popAssumption() { inAssumptions[var(assumptions.last())]=false; assumptions.pop(); }
    inline void pushAssumption(Lit l) { inAssumptions[var(l)]=true; assumptions.push(l); }
    bool resetAndCallSolver();

public:
    inline EnumerationSolver() : Solver(), numberOfModels(0), maxModels(UINT_MAX) { setIncrementalMode(); }
    inline int getLevel(Lit l) const { return level(var(l)); }
    inline bool isChoiceNoAssum(Lit l) const { return !inAssumptions[var(l)] && reason(var(l)) == CRef_Undef; }
    inline Lit createLitFromInt(int l) const { return l > 0 ? mkLit(l,false) : mkLit(-l,true); }
    inline int createIntFromLit(Lit l) const { return sign(l) ? -var(l) : var(l); }

    inline int getMaxLevelOfConflict() const {
        int max=0;
        for(int i=0; i < conflict.size(); i++)
            if(getLevel(conflict[i]) > max)
                max=getLevel(conflict[i]);
        return max;
    }

    bool foundModel();
    void flipLatestChoice();
    void enumerate();
};
