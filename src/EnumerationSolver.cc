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
    cout << "c Answer: " << ++numberOfModels << endl;
    cout << "v";
    for(int i=0; i<nVars(); i++) {
        cout << " " << (modelValue(i) == l_True ? "" : "-") << (i+1); //print
        Lit l =(modelValue(i) == l_True ? mkLit(i,false) : mkLit(i,true));
        if(isChoice(l) && !isAssumption(l)) pushAssumption(l); //all choices that are not in the assumptions are added
    }
    cout << " 0" << endl;
    if( numberOfModels >= maxModels ) return false; //if the number of models to be printed is reached then stop
    return true;
}

void EnumerationSolver::flipLatestChoice() {
    unsigned int size;
    while(true) {
        size=assumptions.size();
        if(size==0) return; //no more assumptions: stop
        if(checked[size-1]) { popAssumption(); } //skip already flipped choices
        else break;
    }
    
    assumptions[size-1] = ~assumptions[size-1]; //flip latest choice
    checked[size-1]=true; //set as flipped
    for(int i = size; i < checked.size(); i++)
        checked[i]=false; //restoring
}

int EnumerationSolver::getBackjumpingLevel() const {
        int max=0;
        for(int i=0; i < conflict.size(); i++)
            if(inAssumptions[var(conflict[i])] > max)
                max=inAssumptions[var(conflict[i])];
        return max;
}

unsigned int EnumerationSolver::enumerate() {
    if(useBlockingClauses_) return enumerateBlockingClause();

    cout << "QUI" << endl;
    assumptions.clear();
    while(checked.size() < nVars()) checked.push(false);     //init vector
    while(inAssumptions.size() < nVars()) inAssumptions.push(-1);     //init vector

    begin:;
    if(!resetAndCallSolver()) {  //call the solver
        //incosistent
        if(conflict.size()==0) goto end; //the conflict does not depend on the assumptions: stop.
        int bl=getBackjumpingLevel(); //compute the meaningful level to backjump to
        while(assumptions.size()!=0 && assumptions.size() > bl) popAssumption(); //clear useless assumptions
    }
    else if(!foundModel()) goto end; //print model and compute assumptions
    flipLatestChoice(); //flip latest assumption
    if(assumptions.size()==0) goto end; //no more assumptions: stop
    goto begin; //repeat the search

    end:;
    return numberOfModels;
}

unsigned int EnumerationSolver::enumerateBlockingClause() {
    while(resetAndCallSolver()) {
        cout << "c Answer: " << ++numberOfModels << endl;
        cout << "v";
        vec<Lit> blockingClause;
        for(int i=0; i<nVars(); i++) {
            cout << " " << (modelValue(i) == l_True ? "" : "-") << (i+1); //print
            Lit l =(modelValue(i) == l_True ? mkLit(i,false) : mkLit(i,true));
            if(isChoice(l)) blockingClause.push(~l); //all choices are added to the clause
        }
        cout << " 0" << endl;
        addClause(blockingClause);
        if( numberOfModels >= maxModels ) break; //if the number of models to be printed is reached then stop
    }
    return numberOfModels;
}
