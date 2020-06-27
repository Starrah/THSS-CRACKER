//
// DPLL algorithm.
//

#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include <set>
#include <algorithm>
#include "common.h"




class ConflictNode
{
public:
    //当前节点
    int Value;
    //祖先节点
    std::vector<int> RootList;
    ConflictNode(int TheValue, std::vector<int>TheRootList)
    {
        Value = TheValue;
        RootList = TheRootList;
    }
};

class ConflictMap 
{
public:
    std::vector<ConflictNode> NodeList;

    /*
    描述：新建节点
    参数：新节点的编号
    返回：无
    */
    void CreateNode(int NewNum);
    /*
    描述：新建节点
    参数：新节点的编号，新节点父亲的编号列表
    返回：无
    */
    void CreateNode(int NewNum, std::vector<int> FatherList);
};




class DPLL 
{
public:
    //辅助数据结构
    //每个元素的定义,1:true -1:false 0:未定义
    std::vector<int> ElementDefinations;
    //所有赋值的情况，按照时间顺序
    std::vector<literal> DefinedElements;
    //所有决策在赋值表的顺序
    std::vector<int> DecisionPlaces;
    ConflictMap TheMap;

    /*
    描述：初始化函数
    参数：无
    返回：无
    */
    void Initialize(); 

    /*
    描述：赋值函数
    参数：被赋值的元素，值
    返回：无
    */
    void SetValue(int Element, bool Value);


    /*
    描述：找到下一个要决策的位置
    参数：无
    返回：下一个位置,找不到返回0
    */
    int FindDecision();

    /*
    描述：决策函数
    参数：被决策的元素，值
    返回：无
    */
    void Decide(int Element, bool Value);


    /*
    描述：给定当前子句的变量literal，判别当前这个literal是什么数值
    参数：当前子句的变量literal
    返回：如果true返回1，否则0
    */
    bool GetValue(int TheVar);

    /*
    描述：传递一个子句
    参数：子句id
    返回：如果可以propagate返回1，否则0
    */
    bool PropagateOne(int ClauseNumber);
    
    /*
    描述：判断一个子句是否满足
    参数：子句id
    返回：正确1，错误-1，未定0
    */
    int JudgeSatisfaction(int ClauseNumber);


    /*
    描述：传递函数
    参数：无
    返回：如果这一次propagate有元素更新，返回1，意为下一轮需要迭代；没有更新返回0，意为propagate停止进行；
    如果遇到矛盾返回-1，意为需要回溯；
    */
    int Propagate();

    /*
    描述：回退函数
    参数：无
    返回：正常：1，没法backtrack了：0
    */
    bool Backtrack();

    /*
    描述：回退函数
    参数：冲突子句的编号
    返回：正常：1，没法backjump了：0
    */
    bool Backjump(int ConflictClause);

    /*
    描述：找到一个公式里有多少已经定义的元素
    参数：公式编号
    返回：定义的元素个数
    */
    int GetDefinedNumInClause(int ClauseNumber);

    /*
    描述：输出当前信息
    参数：无
    返回：无
    */
    void PrintInfo();


    /**
     * Constructor.
     *
     * @param phi the formula to be checked
     * @note Please DON'T CHANGE this signature because the grading script will directly call this function!
     */
    DPLL(const formula &phi) : phi(phi) {Initialize();}

    /**
     * Check if the formula is satisfiable.
     *
     * @return true if satisfiable, and false if unsatisfiable
     * @note Please DON'T CHANGE this signature because the grading script will directly call this function!
     */
    bool check_sat();

    /**
     * Get a satisfying model (interpretation) of the formula, the model must be *complete*, that is,
     * it must assign every variable a truth value.
     * This function will be called if and only if `check_sat()` returns true.
     *
     * @return an arbitrary (if there exist many) satisfying model
     * @note Please DON'T CHANGE this signature because the grading script will directly call this function!
     */
    model get_model();

private:
    formula phi;
};


#endif //DPLL_DPLL_H
