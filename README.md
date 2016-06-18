# ModelsEnumeration
A simple tool for enumerating models based on glucose 4.

Usage
===
```
    EnumerationSolver s;
    for(int i = 0; i < 3; i++)
        s.newVar();

    //add clause 0 v 1 v 2
    vec<Lit> clause;
    for(int i = 0; i < 3; i++)
        clause.push(mkLit(i,false));

    vec<Lit> clause2;
    for(int i = 0; i < 3; i++)
        clause2.push(mkLit(i,true));

    s.addClause(clause);
    s.addClause(clause2);
    s.enumerate();
```
