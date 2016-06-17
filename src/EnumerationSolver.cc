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
#include "EnumerationSolver.h"

#include <iostream>
using namespace std;

bool EnumerationSolver::foundModel() {
    cout << "c num " << ++numberOfModels << endl;
    cout << "v";
    for(int i=0; i<nVars(); i++) {
        int lit=(modelValue(i) == l_True ? i : -i);
        Lit l = createLitFromInt(lit);
        cout << " " << (modelValue(i) == l_True ? "" : "-") << i; //print
        if(isChoiceNoAssum(l)) pushAssumption(l); //all choices that are not in the assumptions are added
    }
    cout << endl;
    if( numberOfModels >= maxModels ) return false; //if the number of models to be printed is reached then stop
    return true;
}

void EnumerationSolver::flipLatestChoice() {
    unsigned int size;
    while(true) {
        size=assumptions.size();
        if(size==0) return; //no more assumptions: stop
        if(checked[size-1]) { checked[size-1] = false; popAssumption(); } //skip already flipped choices
        else break;
    }
    
    assumptions[size-1] = ~assumptions[size-1]; //flip latest choice
    checked[size-1]=true; //set as flipped
}

bool EnumerationSolver::resetAndCallSolver() {
    budgetOff();
    cancelUntil(0); //unroll
    return solve_() == l_True;  //search for a model under assumptions
}

void EnumerationSolver::enumerate() {
    assumptions.clear();
    while(checked.size() < nVars()) checked.push(false);     //init vector
    while(inAssumptions.size() < nVars()) inAssumptions.push(false);     //init vector

    begin:;
    if(!resetAndCallSolver()) {  //call the solver
        //incosistent
        if(conflict.size()==0) return; //the conflict does not depend on the assumptions: stop.
        int bl=getMaxLevelOfConflict(); //compute the meaningful level to backjump to
        while(assumptions.size()!=0 && getLevel(assumptions.last()) > bl) popAssumption(); //clear useless assumptions
    }
    else if(!foundModel()) return; //print model and compute assumptions
    flipLatestChoice(); //flip latest assumption
    if(assumptions.size()==0) return; //no more assumptions: stop
    goto begin; //repeat the search
}
