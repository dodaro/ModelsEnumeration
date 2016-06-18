# ModelsEnumeration
A simple tool for enumerating models based on glucose 4.

Usage as black box
===
To compile just type
```
make
```
Then, you can use either a file or stdin
```
./build/release/enumerator dimacs.file
./build/release/enumerator < dimacs.file
```

Usage in other projects
===
The EnumerationSolver can be used as the simple solver of glucose.
```
    EnumerationSolver s;
    for(int i = 0; i < 3; i++)
        s.newVar();

    //add clause 1 v 2 v 3
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
