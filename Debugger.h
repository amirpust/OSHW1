#ifndef OSHW1_DEBUGGER_H
#define OSHW1_DEBUGGER_H


static int debugScope = 0;

#define PRINT_START do{                             \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"Start :"<< __FUNCTION__ << endl;    \
        debugScope++;                              \
        }while(0)


#define PRINT_END do{                             \
        debugScope--;                               \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"END :"<< __FUNCTION__ << endl;    \
        }while(0)



#define PRINT_START_PARAM(param) do{    \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"Start :"<< __FUNCTION__ << " With " << #param << ": " << param << endl; \
        debugScope++;                               \
        }while(0)


#define PRINT_END_PARAM(param) do{    \
        debugScope--;                                \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"END :"<< __FUNCTION__ << " With " << #param << ": " << param << endl; \
        }while(0)

#define PRINT_PARAM(param) do{    \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout << #param << ": " << param << endl; \
        }while(0)



#endif //OSHW1_DEBUGGER_H
