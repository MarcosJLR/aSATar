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

        if(clauses[i][0] == p) { std::swap(clauses[i][0], clauses[i][1]); }

        if(isTrue(clauses[i][0])) { return false; }

        for(uint j = 2; j < clauses[i].size(); j++)
        {
            if(isTrue(clauses[i][j]))
            {
                watchList[toInt(clauses[i][j])].push_back(i);
                std::swap(clauses[i][1], clauses[i][j]);
                std::swap(clauses[i][0], clauses[i][1]);
                return true;
            }
            if(isUndef(clauses[i][j]))
            {
                watchList[toInt(clauses[i][j])].push_back(i);
                std::swap(clauses[i][1], clauses[i][j]);
                if(isFalse(clauses[i][0]))
                {
                    std::swap(clauses[i][0], clauses[i][1]);
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

    Var Solver::selectNextVar()
    {
        while(assign[nextVar] == -1) { nextVar++; }

        return nextVar;
    }

    bool Solver::backTrack()
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

        nextVar = x + 1;

        return false;
    }

    bool Solver::solve()
    {
        while(true)
        {
            Var x = selectNextVar();
            makeDecision(x);
            while(true)
            {
                bool posible = unitPropagation();
                if(!posible)
                {
                    bool atLevel0 = backTrack();
                    if(atLevel0) { ok = 0; return UNSAT; }
                }
                else if(isSAT())
                {
                    ok = 1;
                    return SAT; 
                }
                else 
                { 
                    break; 
                }
            }
        }
    }

    void Solver::readFromFile(std::ifstream& file)
    {
        std::string line = "";
        std::stringstream ss;

        while(getline(file, line))
        {
            if(line[0] == 'p') { break; }
        }

        ss << line;

        ss >> line;        
        assert(line == "p");

        ss >> line;        
        assert(line == "cnf");

        ss >> N >> M;

        uint m = 0;
        int l = 0;
        while(file >> l)
        {
            if(l != 0)
            {
                Var x = abs(l) - 1;
                Lit p = mkLit(x, l < 0);
                clauses[m].push_back(p);
            }
            else { m++; }
        }

        assert(M == m+1);
    }

    void Solver::readFromFile(const std::string filename)
    {
        std::ifstream file(filename);
        readFromFile(file);
        file.close();
    }

    void Solver::printToFile(std::ofstream& file)
    {
        file << "c Solution created by aSATar: The Last SAT Solver" << std::endl;
        file << "c " << std::endl;
        file << "s cnf " << ok << N << std::endl;

        if(ok != 1) { return; }

        for(uint i = 0; i < N; i++)
        {
            int x = i + 1;
            if(assign[i] == 0) { x = -x; }
            file << "v " << x << std::endl;
        }
    }

    void Solver::printToFile(const std::string filename)
    {
        std::ofstream file(filename);
        printToFile(file);
        file.close();
    }
};