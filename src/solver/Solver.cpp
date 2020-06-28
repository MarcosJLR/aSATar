#include "Solver.hpp"

namespace asatar
{
    bool Solver::unitPropagation()
    {
        while(!unitClauses.empty())
        {
            int i = unitClauses.top();
            unitClauses.pop();

            if(empty(clauses[i]))
            {
                // Clear Unit clauses stack and return false
                // meaning a conflict has occurred
                while(!unitClauses.empty()) { unitClauses.pop(); }
                return false;
            }

            setVariable(clauses[i][0]);
        }

        return true;
    }

    void Solver::setVariable(Lit p)
    {
        changeStack.push(var(p));
        assign[var(p)] = sign(~p);
        auto it = watchList[toInt(~p)].begin();

        while(it != watchList[toInt(~p)].end())
        {
            if(updateWatcher(*it, ~p))
            {
                it = watchList[toInt(~p)].erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    bool Solver::updateWatcher(const int i, Lit p)
    {
        if(clauses[i].size() < 2) { return false; } 

        if(clauses[i][0] == p) { swap(clauses[i][0], clauses[i][1]); }

        if(isTrue(clauses[i][0])) { return false; }

        for(uint j = 2; j < clause.size(); j++)
        {
            if(isTrue(clauses[i][j]))
            {
                watchList[toInt(clauses[i][j])].push_back(i);
                swap(clauses[i][1], clauses[i][j]);
                swap(clauses[i][0], clauses[i][1]);
                return true;
            }
            if(isUndef(clauses[i][j]))
            {
                watchList[toInt(clauses[i][j])].push_back(i);
                swap(clauses[i][1], clauses[i][j]);
                if(isFalse(clauses[i][0]))
                {
                    swap(clauses[i][0], clauses[i][1]);
                }                
                return true;
            }
        }

        if(isUndef(clauses[i][0])) { unitClauses.push(i); }

        return false;
    }

    void Solver::makeDecision(Var x)
    {
        decisionStack.push(x);
        setVariable(mkLit(x));
    }

    bool Solver::backtrack()
    {
        if(decisionStack.empty())
        {
            return true;
        }

        Var x = decisionStack.top();
        decisionStack.pop();

        while(changeStack.top() != x)
        {
            Var y = changeStack.top();
            changeStack.pop();
            assign[y] = -1;
        }
        changeStack.pop();

        setVariable(mkLit(x, assign[x]));

        return false;
    }

    bool Solver::solve()
    {
        while(true)
        {
            // Decide what variable to set
            while(true)
            {
                bool posible = unitPropagation();
                if(!posible)
                {
                    bool atLevel0 = backtrack();
                    if(atLevel0) { return UNSAT; }
                }
                else if(isSAT())
                {
                    return SAT; 
                }
                else 
                { 
                    break; 
                }
            }
        }
    }
};