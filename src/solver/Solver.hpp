#pragma once

#include <cstring>
#include <cassert>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <stack>
#include <list>
#include <sys/time.h>
#include <sys/resource.h>

#include "SolverTypes.hpp"

#define MAX_VAR     100000
#define MAX_LIT     200000
#define MAX_CLAUSE  200000

namespace asatar
{
    class Solver
    {
    public:
        // Constructor
        // Init sizes to 0 and all assignments to -1 (unassigned)
        Solver() 
        {
            N = M = 0;            
            memset(assign, -1, sizeof(assign));
            hasTimeout = false;
            nextVar = 0;
            ok = -1;
        }

        inline bool isSAT()                 { return changeStack.size() == N; }                 // Is the formula currently satisfied
        inline int value(Lit p)             { return assign[var(p)] == -1 ? -1 : (int)p.eval(assign[var(p)]); } 
        inline bool isTrue(Lit p)           { return value(p) == 1; }                           // Does this literal evaluates to True
        inline bool isFalse(Lit p)          { return value(p) == 0; }                           // Does this literal evaluates to False
        inline bool isUndef(Lit p)          { return value(p) == -1; }                          // Is this literal undefined
        inline bool empty(Clause& clause)   { return clause.empty() || isFalse(clause[0]); }    // Is this clause empty (all literals are false)
        inline bool unit(Clause& clause)    { return !empty(clause) && isUndef(clause[0]) &&    // Is this a unit clause (all but one literal is false)
                                              (clause.size() == 1 || isFalse(clause[1])); }

        bool unitPropagation();                 // Propagate unit clauses
        void setVariable(Lit p);                // Assign value to variable to make p true and handle watchlists
        bool updateWatcher(int i, Lit p);       // Update watcher of clause after value of p changes
        void makeDecision(Var x);               // Make a decision for variable x                
        Var selectNextVar();                    // Select next decision varible
        bool backTrack();                       // Undo all assignments made in the last decision level

        void init();                            // Initialize unit clause stack and watchlists
        bool solve();                           // Solve this instance of SAT

        // IO functions
        void readFromFile(std::istream& file);         // Read from file given filestream
        void readFromFile(const std::string filename);  // Read from file given filename

        void printToFile(std::ostream& file);          // Print to file given filestream
        void printToFile(const std::string filename);   // Print to file given filename

        // Time functions
        bool timeout();                         // Tells if time is out 
        void setTimeout(int newTimeout);        // Sets timeout to be newTimeout

    private:
        uint N;                                 // Number of variables
        uint M;                                 // Number of clauses    
        int assign[MAX_VAR];                    // Current assignment (-1 means this variable is unassigned)
        std::list<int> watchList[MAX_LIT];      // Watch list for literal watchers
        Clause clauses[MAX_CLAUSE];             // Clauses (watched literals are always the first two)
        std::stack<int> unitClauses;            // Unit clauses to propagate

        std::stack<Var> changeStack;            // Stack containing all variable assignments
        std::stack<Var> decisionStack;          // Stack containing all decision variables in order

        Var nextVar;                            // First not yet set variable
        int ok;                                 // 1 if SAT, 0 if UNSAT, -1 if haven't decided yet

        // Time management
        double startTime;
        double finishTime;

        int timeoutDuration;
        bool hasTimeout;

        static const bool UNSOLVED = false;
        static const bool UNSAT = false;
        static const bool SAT   = true;
    };
};