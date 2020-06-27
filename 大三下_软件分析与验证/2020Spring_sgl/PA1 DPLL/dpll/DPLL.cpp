//
// DPLL algorithm.
//

#include "DPLL.h"

//两个vector求并集
std::vector<int> vectors_set_union(std::vector<int> v1, std::vector<int> v2)
{
    std::vector<int> v;
    sort(v1.begin(),v1.end());   
    sort(v2.begin(),v2.end());   
    std::set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));//求交集 
    return v;
}

void ConflictMap::CreateNode(int NewNum)
{
    int flag = 0;
    std::vector<int> Target;
    Target.push_back(NewNum);
    ConflictNode NewConflictNode(NewNum, Target);
    NodeList.push_back(NewConflictNode);
}

void ConflictMap::CreateNode(int NewNum, std::vector<int> FatherList)
{
    int flag = 0;
    std::vector<int> Target;
    if(FatherList.size() == 0)
    {
        Target.push_back(NewNum);
    }
    else
    {
        for(int i = 0; i < NodeList.size(); i ++)
        {
            if(abs(NodeList[i].Value) == abs(FatherList[flag]))
            {
                Target = vectors_set_union(Target, NodeList[i].RootList);
                flag ++;
            }
        }
    }
    ConflictNode NewConflictNode(NewNum, Target);
    NodeList.push_back(NewConflictNode);
}

void DPLL::PrintInfo()
{
    for(int i = 0; i < DefinedElements.size(); i ++)
    {
        printf("%d ",DefinedElements[i]);
    }
    printf("\n");
}

void DPLL::Initialize()
{
    for(int i = 0; i <= phi.num_variable; i ++)
    {
        ElementDefinations.push_back(0);
    }
    DefinedElements.clear();
    DecisionPlaces.clear();
}

void DPLL::SetValue(int Element, bool Value)
{
    if(Value == true)
    {
        ElementDefinations[Element] = 1;
        DefinedElements.push_back(Element);
    }
    else
    {
        ElementDefinations[Element] = -1;
        DefinedElements.push_back(Element * -1);

    }
}

void DPLL::Decide(int Element, bool Value)
{
    SetValue(Element, Value);
    int TheValue = DefinedElements[DefinedElements.size()-1];
    DecisionPlaces.push_back(DefinedElements.size() - 1);
    std::vector<int> FatherList;
    FatherList.push_back(TheValue);
    TheMap.CreateNode(TheValue);
}

int DPLL::FindDecision()
{
    for(int i = 1; i <= phi.num_variable; i ++)
    {
        if(ElementDefinations[i] == 0)
        {
            return i;
        }
    }
    return 0;
}

int DPLL::GetDefinedNumInClause(int ClauseNumber)
{
    int DefinedNum = 0;
    clause TheClause = phi.clauses[ClauseNumber];
    for(int i = 0; i < TheClause.size(); i ++)
    {
        int TheElement = abs(TheClause[i]);
        if(ElementDefinations[TheElement] != 0)
        {
            DefinedNum ++;
        }
    }
    return DefinedNum;
}

bool DPLL::GetValue(int TheVar)
{
    bool WhetherPositive = 1;
    if(TheVar < 0)
    {
        WhetherPositive = 0;
    }
    bool WhetherTrue = 1;
    if(ElementDefinations[abs(TheVar)] == -1)
    {
        WhetherTrue = 0;
    }
    if(WhetherPositive == WhetherTrue)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


bool DPLL::PropagateOne(int ClauseNumber)
{
    int UndefinedNum = 0;
    bool CurrentResult = 0;
    clause TheClause = phi.clauses[ClauseNumber];
    std::vector<int> DefinedNumberList;
    for(int i = 0; i < TheClause.size(); i ++)
    {
        if(ElementDefinations[abs(TheClause[i])] != 0)
        {
            CurrentResult = CurrentResult | GetValue(TheClause[i]);
            DefinedNumberList.push_back(TheClause[i]);
        }
        else
        {
            UndefinedNum = TheClause[i];
        }
    }
    if(CurrentResult == 0)
    {
        if(UndefinedNum > 0)
        {
            SetValue(UndefinedNum, 1);
            TheMap.CreateNode(UndefinedNum, DefinedNumberList);
        }
        else
        {
            SetValue(abs(UndefinedNum), 0);
            TheMap.CreateNode(UndefinedNum, DefinedNumberList);
        }
        
        return 1;
    }
    return 0;
}

int DPLL::JudgeSatisfaction(int ClauseNumber)
{
    bool CurrentResult = 0;
    bool HasUndefined = 0;
    clause TheClause = phi.clauses[ClauseNumber];
    for(int i = 0; i < TheClause.size(); i ++)
    {
        if(ElementDefinations[abs(TheClause[i])] != 0)
        {
            CurrentResult = CurrentResult | GetValue(TheClause[i]);
        }
        else
        {
            HasUndefined = 1;
        }
        
    }
    if(CurrentResult == 0)
    {
        if(HasUndefined == 0)
        {
            return -1;
        }
        else
        {
            return 0;
        }
        
    }
    return 1;
}

int DPLL::Propagate()
{
    bool WhetherRenew = 0;
    for(int i = 0; i < phi.clauses.size(); i ++)
    {
        int Length = phi.clauses[i].size();
        int TheResult = JudgeSatisfaction(i);
        if(TheResult == -1)
        {
            return -1 - i;
        }
        int CurrentNum = GetDefinedNumInClause(i);
        if(TheResult == 0 && CurrentNum == Length - 1)
        {
            
            if(PropagateOne(i))
            {
                WhetherRenew = 1;
                //测试
                //printf("After Propagating one\n");
                //PrintInfo();
            }
        }
    }
    return WhetherRenew;
}

bool DPLL::Backtrack()
{
    if(DecisionPlaces.size() == 0)
    {
        return 0;
    }
    int LastDecisionPlace = DecisionPlaces[DecisionPlaces.size() - 1];
    int LastDecisionNum = DefinedElements[LastDecisionPlace];
    int TotalPlace = DefinedElements.size();
    int PreviousLastDecision = ElementDefinations[LastDecisionNum];
    bool NewDecision = 0;
    if(PreviousLastDecision == -1)
    {
        NewDecision = 1;
    }
    int BacktrackNum = TotalPlace - LastDecisionPlace;
    DecisionPlaces.pop_back();
    for(int i = 0; i < BacktrackNum; i ++)
    {
        int ClearedElement = abs(DefinedElements[DefinedElements.size()-1]);
        ElementDefinations[ClearedElement] = 0;
        DefinedElements.pop_back();
    }
    SetValue(LastDecisionNum, NewDecision);
    return 1;
}


bool DPLL::Backjump(int ConflictClause)
{
    std::vector<int> ConflictRoots;
    clause TheClause = phi.clauses[ConflictClause];
    int PlaceInMap = -1;
    int PlaceInClause = -1;
    //找到冲突元素
    bool Success = 0;
    for(int i = TheMap.NodeList.size() - 1; i >= 0; i --)
    {
        for(int j = 0; j < TheClause.size(); j ++)
        {
            if(abs(TheMap.NodeList[i].Value) == abs(TheClause[j]))
            {
                PlaceInMap = i;
                PlaceInClause = j;
                Success = 1;
            }
            if(Success) break;
        }
        if(Success) break;
    }

    int kebab1 = 0;



    //得到冲突元素的根节点列表1
    for(int i = 0; i < TheClause.size(); i ++)
    {
        if(i == PlaceInClause)
        {
            continue;
        }
        int TheElement = abs(TheClause[i]);
        int TheElementWithValue = ElementDefinations[TheElement] * TheElement;
        for(int j = TheMap.NodeList.size() - 1; j >= 0; j --)
        {
            if(TheMap.NodeList[j].Value == TheElementWithValue)
            {
                ConflictRoots = vectors_set_union(ConflictRoots, TheMap.NodeList[j].RootList);
                break;
            }
        }
    }
    //合并冲突元素的根节点列表2
    ConflictRoots = vectors_set_union(ConflictRoots, TheMap.NodeList[PlaceInMap].RootList);
    
    //找到回退位置
    int BackjumpPlace = -1;
    int DecisionBackjumpPlace = -1;
    int LastDecisionPlace = -1;
    int LastDecisionNum = 0;
    int BeforeLastDecisionPlace = -1;
    int BeforeLastDecisionNum = 0;
    int ChangedPlace = -1;
    Success = 0;
    for(int i = DecisionPlaces.size() - 1; i >= 0; i --)
    {
        for(int j = 0; j < ConflictRoots.size(); j ++)
        {
            int DecideNum = DefinedElements[DecisionPlaces[i]];
            int ConflictNum = ConflictRoots[j];
            if(abs(DecideNum) == abs(ConflictNum))
            {
                if(LastDecisionPlace == -1)
                {
                    LastDecisionPlace = DecisionPlaces[i];
                    LastDecisionNum = ConflictRoots[j];
                    ChangedPlace = j;
                }
                else if(BeforeLastDecisionPlace == -1)
                {
                    BeforeLastDecisionPlace = DecisionPlaces[i];
                    BeforeLastDecisionNum = ConflictRoots[j];
                    BackjumpPlace = DecisionPlaces[i + 1];
                    DecisionBackjumpPlace = i + 1;
                    Success = 1;
                }
                break;
            }
            if(Success) break;
        }
        if(Success) break;
    }
    if(LastDecisionPlace == -1)
    {
        return 0;
    }
    if(BeforeLastDecisionPlace == -1)
    {
        BackjumpPlace = 0;
        DecisionBackjumpPlace = 0;
    }


    //清除元素
    int BacktrackNum = DefinedElements.size() - BackjumpPlace;
    for(int i = 0; i < BacktrackNum; i ++)
    {
        int ClearedElement = abs(DefinedElements[DefinedElements.size() - 1]);
        ElementDefinations[ClearedElement] = 0;
        DefinedElements.pop_back();
        TheMap.NodeList.pop_back();
    }
    int DecisionBacktrackNum = DecisionPlaces.size() - DecisionBackjumpPlace;
    for(int i = 0; i < DecisionBacktrackNum; i ++)
    {
        DecisionPlaces.pop_back();
    }

    //新增对应元素,等价于propagate一个元素
    std::vector<int> NewConflictRoot;
    for(int i = 0; i < ConflictRoots.size(); i ++)
    {
        if(i == ChangedPlace)
        {
            continue;
        }
        NewConflictRoot.push_back(ConflictRoots[i]);
    }
    int NewValue = 0 - ConflictRoots[ChangedPlace];
    bool NewBoolValue = 1;
    if(NewValue < 0)
    {
        NewBoolValue = 0;
    }
    SetValue(abs(NewValue), NewBoolValue);
    TheMap.CreateNode(NewValue, NewConflictRoot);

    //新增产生式
    std::vector<int> NewFormula;
    for(int i = 0; i < ConflictRoots.size(); i ++)
    {
        NewFormula.push_back(0 - ConflictRoots[i]);
    }
    phi.clauses.push_back(NewFormula);
    return 1;
}

bool DPLL::check_sat() 
{
    while(1)
    {
        int NextDecision = FindDecision();
        if(NextDecision == 0)
        {
            return 0;
        }
        Decide(NextDecision, 1);
        //测试
        //printf("After Decision\n");
        //PrintInfo();

        while(1)
        {
            int result = Propagate();
            if(result == 0)
            {
                if(DefinedElements.size() >= phi.num_variable)
                {
                    return 1;
                }
                break;
            }
            else if(result < 0)
            {
                int ConflictClauseNumber = -1 - result;
                if(Backjump(ConflictClauseNumber) == 0)
                {
                    return 0;
                }
            }
        }

    }
}

model DPLL::get_model() 
{
    model Result;
    for(int i = 1; i <= phi.num_variable; i ++)
    {   
        bool Value = 1;
        if(ElementDefinations[i] == -1)
        {
            Value = 0;
        }
        Result.insert(std::pair<int,bool>(i, Value));
    }
    return Result;
}