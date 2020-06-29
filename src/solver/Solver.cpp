#include "Solver.hpp"

namespace asatar
{
    bool Solver::unitPropagation()
    {
        while(!unitClauses.empty())
        {
            int i = unitClauses.top();
            unitClauses.pop();

            if(isFalse(clauses[i][0]))
            {
                // Clear Unit clauses stack and return false
                // meaning a conflict has occurred
                while(!unitClauses.empty()) { unitClauses.pop(); }
                return false;
            }

            if(isUndef(clauses[i][0])) { setVariable(clauses[i][0]); }
        }

        return true;
    }

    void Solver::setVariable(Lit p)
    {
        assert(isUndef(p));
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
        while(assign[nextVar] != -1) { nextVar++; }

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

        Lit p = mkLit(x, assign[x]);
        assign[x] = -1;

        setVariable(p);

        nextVar = x + 1;

        return false;
    }

    void Solver::init()
    {
        for(uint i = 0; i < M; i++)
        {
            if(clauses[i].size() == 1) 
            { 
                watchList[toInt(clauses[i][0])].push_back(i);
                unitClauses.push(i); 
            }
            else
            {
                watchList[toInt(clauses[i][0])].push_back(i);
                watchList[toInt(clauses[i][1])].push_back(i);
            }
        }
    }

    bool Solver::solve()
    {
        startTime = std::chrono::steady_clock::now();

        init();

        if(!unitPropagation()) 
        {
            finishTime = std::chrono::steady_clock::now();
            ok = 0; 
            return UNSAT; 
        }
        if(isSAT()) 
        {
            finishTime = std::chrono::steady_clock::now();
            ok = 1; 
            return SAT; 
        }

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
                    if(atLevel0) 
                    { 
                        finishTime = std::chrono::steady_clock::now();
                        ok = 0; 
                        return UNSAT; 
                    }
                }
                else if(isSAT())
                {
                    finishTime = std::chrono::steady_clock::now();
                    ok = 1;
                    return SAT; 
                }
                else 
                { 
                    break; 
                }
                if(timeout())
                {
                    finishTime = std::chrono::steady_clock::now();
                    return UNSOLVED;                    
                }
            }
        }
    }

    void Solver::readFromFile(std::istream& file)
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

    void Solver::printToFile(std::ostream& file)
    {
        auto totalRuntime = std::chrono::duration_cast<std::chrono::milliseconds>( finishTime - startTime ).count();

        file << "c Solution created by aSATar: The Last SAT Solver" << std::endl;
        file << "c rt " << totalRuntime << std::endl;
        file << "c " << std::endl;
        file << "s cnf " << ok << " " << N << std::endl;

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

    bool Solver::timeout()
    {
        if(!hasTimeout) { return false; }
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>( currentTime - startTime ) > timeoutDuration;
    }

    void Solver::setTimeout(int newTimeout)
    {
        hasTimeout = true;
        timeoutDuration = std::chrono::milliseconds(newTimeout);
    }
};