#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <map>
#include <stdio.h>

enum TokenType
{
    ID,
    NUM,
    PRINT,
    SEMICOLON,
    COMMA,
    PLUS,
    EQUAL,
    LBRACKET,
    RBRACKET,
    TERMINAL,   // 以上是终结符

    S,          // 以下是非终结符
    E,
    L,

    ED
};

enum ActionType
{
    SN,
    GN,
    RK,
    AC
};

std::string ActionType2Str(ActionType a)
{
    switch (a)
    {
        case SN:
            return "S";
        case GN:
            return "G";
        case RK:
            return "R";
        case AC:
            return "A";
    }
}

const int NUM_OF_STATES  = 23;
const int ERR_STATE = -1;
int STATE(int state) { return state - 1; }
int RULE(int r) { return r - 1; }
void print_rule(int r)
{
    switch (r)
    {
        case 0:
            std::cout << "S->S;S";
            break;
        case 1:
            std::cout << "S->id:=E";
            break;
        case 2:
            std::cout << "print(L)";
            break;
        case 3:
            std::cout << "E->id";
            break;
        case 4:
            std::cout << "E->num";
            break;
        case 5:
            std::cout << "E->E+E";
            break;
        case 6:
            std::cout << "(S, E)";
            break;
        case 7:
            std::cout << "L->E";
            break;
        case 8:
            std::cout << "L->L, E";
            break;
    }
}
struct action
{
    ActionType act;
    int state;
    action(ActionType act, int s) : act(act), state(s) {}
    action() : state(ERR_STATE) {}
};

struct token
{
    TokenType tp;
    int state;
    token(){}
    token(TokenType t, int s) : tp(t), state(s) {}
};


action action_tbl[ED][NUM_OF_STATES];
std::vector<TokenType > rule[9];

void init_action_tbl()
{
    for (size_t i = 0; i < ED; ++i)
    {
        for (size_t j = 0; j < NUM_OF_STATES; ++j)
        {
            action_tbl[i][j].state = ERR_STATE;
        }
    }

    action_tbl[ID][STATE(1)]    = action(SN, 4);
    action_tbl[ID][STATE(3)]    = action(SN, 4);
    action_tbl[ID][STATE(6)]    = action(SN, 20);
    action_tbl[ID][STATE(8)]    = action(SN, 4);
    action_tbl[ID][STATE(16)]   = action(SN, 20);
    action_tbl[ID][STATE(18)]   = action(SN, 20);
    action_tbl[ID][STATE(19)]   = action(SN, 20);

    action_tbl[NUM][STATE(6)]    = action(SN, 10);
    action_tbl[NUM][STATE(16)]   = action(SN, 10);
    action_tbl[NUM][STATE(18)]   = action(SN, 10);
    action_tbl[NUM][STATE(19)]   = action(SN, 10);

    action_tbl[PRINT][STATE(1)]   = action(SN, 7);
    action_tbl[PRINT][STATE(3)]   = action(SN, 7);
    action_tbl[PRINT][STATE(8)]   = action(SN, 7);

    action_tbl[SEMICOLON][STATE(2)]   = action(SN, 3);
    action_tbl[SEMICOLON][STATE(5)]   = action(RK, 1);
    action_tbl[SEMICOLON][STATE(10)]   = action(RK, 5);
    action_tbl[SEMICOLON][STATE(11)]   = action(RK, 2);
    action_tbl[SEMICOLON][STATE(12)]   = action(SN, 3);
    action_tbl[SEMICOLON][STATE(13)]   = action(RK, 3);
    action_tbl[SEMICOLON][STATE(17)]   = action(RK, 6);
    action_tbl[SEMICOLON][STATE(20)]   = action(RK, 4);
    action_tbl[SEMICOLON][STATE(22)]   = action(RK, 7);

    action_tbl[COMMA][STATE(5)]    = action(RK, 1);
    action_tbl[COMMA][STATE(10)]   = action(RK, 5);
    action_tbl[COMMA][STATE(11)]   = action(RK, 2);
    action_tbl[COMMA][STATE(12)]   = action(SN, 18);
    action_tbl[COMMA][STATE(13)]   = action(RK, 3);
    action_tbl[COMMA][STATE(14)]   = action(SN, 19);
    action_tbl[COMMA][STATE(15)]   = action(RK, 8);
    action_tbl[COMMA][STATE(17)]   = action(RK, 6);
    action_tbl[COMMA][STATE(20)]   = action(RK, 4);
    action_tbl[COMMA][STATE(22)]   = action(RK, 7);
    action_tbl[COMMA][STATE(23)]   = action(RK, 9);

    action_tbl[PLUS][STATE(10)]   = action(RK, 5);
    action_tbl[PLUS][STATE(11)]   = action(SN, 16);
    action_tbl[PLUS][STATE(17)]   = action(SN, 16);
    action_tbl[PLUS][STATE(20)]   = action(RK, 4);
    action_tbl[PLUS][STATE(22)]   = action(RK, 17);
    action_tbl[PLUS][STATE(23)]   = action(SN, 16);

    action_tbl[EQUAL][STATE(4)]   = action(SN, 6);

    action_tbl[LBRACKET][STATE(6)]    = action(SN, 8);
    action_tbl[LBRACKET][STATE(7)]    = action(SN, 9);
    action_tbl[LBRACKET][STATE(16)]   = action(SN, 8);
    action_tbl[LBRACKET][STATE(18)]   = action(SN, 8);
    action_tbl[LBRACKET][STATE(19)]   = action(SN, 8);

    action_tbl[RBRACKET][STATE(10)]   = action(RK, 5);
    action_tbl[RBRACKET][STATE(14)]   = action(SN, 13);
    action_tbl[RBRACKET][STATE(15)]   = action(RK, 8);
    action_tbl[RBRACKET][STATE(17)]   = action(RK, 6);
    action_tbl[RBRACKET][STATE(20)]   = action(RK, 4);
    action_tbl[RBRACKET][STATE(21)]   = action(SN, 22);
    action_tbl[RBRACKET][STATE(22)]   = action(RK, 7);
    action_tbl[RBRACKET][STATE(23)]   = action(RK, 9);

    action_tbl[TERMINAL][STATE(2)]    = action(AC, 0);
    action_tbl[TERMINAL][STATE(5)]    = action(RK, 1);
    action_tbl[TERMINAL][STATE(10)]   = action(RK, 5);
    action_tbl[TERMINAL][STATE(11)]   = action(RK, 2);
    action_tbl[TERMINAL][STATE(13)]   = action(RK, 3);
    action_tbl[TERMINAL][STATE(17)]   = action(RK, 6);
    action_tbl[TERMINAL][STATE(20)]   = action(RK, 4);
    action_tbl[TERMINAL][STATE(22)]   = action(RK, 7);

    //
    action_tbl[S][STATE(1)]   = action(GN, 2);
    action_tbl[S][STATE(3)]   = action(GN, 5);
    action_tbl[S][STATE(8)]   = action(GN, 12);

    action_tbl[E][STATE(6)]    = action(GN, 11);
    action_tbl[E][STATE(9)]    = action(GN, 15);
    action_tbl[E][STATE(16)]   = action(GN, 17);
    action_tbl[E][STATE(18)]   = action(GN, 21);
    action_tbl[E][STATE(19)]   = action(GN, 23);

    action_tbl[L][STATE(9)]   = action(GN, 14);
}

void init_rule()
{
    rule[0].push_back(S);
    rule[0].push_back(S);
    rule[0].push_back(SEMICOLON);
    rule[0].push_back(S);

    rule[1].push_back(S);
    rule[1].push_back(ID);
    rule[1].push_back(EQUAL);
    rule[1].push_back(E);

    rule[2].push_back(S);
    rule[2].push_back(PRINT);
    rule[2].push_back(LBRACKET);
    rule[2].push_back(L);
    rule[2].push_back(RBRACKET);

    rule[3].push_back(E);
    rule[3].push_back(ID);

    rule[4].push_back(E);
    rule[4].push_back(NUM);

    rule[5].push_back(E);
    rule[5].push_back(E);
    rule[5].push_back(PLUS);
    rule[5].push_back(E);

    rule[6].push_back(E);
    rule[6].push_back(LBRACKET);
    rule[6].push_back(S);
    rule[6].push_back(COMMA);
    rule[6].push_back(E);
    rule[6].push_back(RBRACKET);

    rule[7].push_back(L);
    rule[7].push_back(E);

    rule[8].push_back(L);
    rule[8].push_back(L);
    rule[8].push_back(E);

}

int parse(std::vector<TokenType > input)
{
    std::stack<token > stack;
    stack.push(token(ED, 1));
    size_t idx = 0;
    while (true)
    {
        if (idx >= input.size()) return -1;
        auto cur = input[idx];
        // 查表
        {
            auto sta = stack.top().state;
            auto act = action_tbl[cur][STATE(sta)];
            if (act.state == ERR_STATE) return -1;
            switch (act.act)
            {
                case SN:
                {
                    std::cout << "移进 " << act.state;
                    std::cout << std::endl;
                    stack.push(token(cur, act.state));
                    ++idx;
                }
                    break;
                case RK:
                {

                    auto k = act.state;
                    auto ru = rule[RULE(k)];
                    int popnum = ru.size() - 1;
                    auto ll = ru.front();

                    {
                        std::cout << "规约 ";
                        print_rule(RULE(k));
                        std::cout << std::endl;
                    }

                    for (size_t ini = 0; ini < popnum; ++ini)
                        stack.pop();

                    if (stack.size() <= 0) return -1;
                    {
                        auto act = action_tbl[ll][STATE(stack.top().state)];
                        stack.push(token(ll, act.state));
                    }

                }
                    break;
                case AC:
                {
                    std::cout << "接收";
                    return 0;
                }
                    break;
                default:
                    return -1;
            }
        }
    }

    return -1;
}


// 语法分析
namespace test
{

    enum TokenType
    {
        S1,
        S,
        V,
        E,
        X,
        STAR,
        EQUAL,
        TERMINAL
    };

    std::string TokenType2Str(TokenType t)
    {
        switch (t)
        {
            case S1:
                return "S'";
            case S:
                return "S";
            case V:
                return "V";
            case E:
                return "E";
            case X:
                return "X";
            case STAR:
                return "*";
            case EQUAL:
                return "=";
            case TERMINAL:
                return "$";
            default:
                return "";
        }
    }

    /*enum TokenType
    {
        E1,
        E,
        ID,
        NUM,
        STAR,
        CHU,
        PLUS,
        JIAN,
        LBRA,
        RBRA,
        TERMINAL
    };

    std::string TokenType2Str(TokenType t)
    {
        switch (t)
        {
            case E1:
                return "E'";
            case E:
                return "E";
            case ID:
                return "ID";
            case NUM:
                return "NUM";
            case STAR:
                return "*";
            case CHU:
                return "/";
            case PLUS:
                return "+";
            case JIAN:
                return "-";
            case LBRA:
                return "(";
            case RBRA:
                return ")";
            case TERMINAL:
                return "$";
            default:
                return "";
        }
    }*/

    std::string TokenTypes2Str(std::vector<TokenType > in)
    {
        std::string ret = "";
        for (auto it : in)
        {
            ret += TokenType2Str(it);
        }
        return ret;
    }

    std::vector<std::vector<TokenType >> rule;
    std::vector<std::vector<action >> tbl;
    struct Item
    {
        TokenType A;
        std::vector<TokenType > alpha;
        std::vector<TokenType > beta;
        TokenType x;

        bool operator<(const Item& rh) const
        {
            if (A < rh.A)
                return true;
            else if (A > rh.A)
                return false;
            else
            {
                if (x < rh.x)
                    return true;
                else if (x > rh.x)
                    return false;
                else
                {
                    if (alpha.size() < rh.alpha.size())
                        return true;
                    else if (alpha.size() > rh.alpha.size())
                        return false;
                    else
                    {
                        if (alpha == rh.alpha)
                        {
                            if (beta.size() < rh.beta.size())
                                return true;
                            else if (beta.size() > rh.beta.size())
                                return false;
                            else
                            {
                                if (beta == rh.beta)
                                    return false;
                                auto it1 = beta.begin();
                                auto it2 = rh.beta.begin();
                                while (it1 != beta.end() && it2 != rh.beta.end())
                                {
                                    if (*it1 < *it2)
                                        return true;
                                    else if (*it1 > *it2)
                                        return false;
                                    else
                                    {
                                        it1++;it2++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            auto it1 = alpha.begin();
                            auto it2 = rh.alpha.begin();
                            while (it1 != alpha.end() && it2 != rh.alpha.end())
                            {
                                if (*it1 < *it2)
                                    return true;
                                else if (*it1 > *it2)
                                    return false;
                                else
                                {
                                    it1++;it2++;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }
        bool operator==(const Item& rh) const
        {
            return !(*this < rh) && !(rh < *this);
        }
        void print() const
        {
            std::string ret = "";
            ret += TokenType2Str(A);
            ret += "->";
            ret += TokenTypes2Str(alpha);
            ret += ".";
            ret += TokenTypes2Str(beta);
            ret += "    ";
            ret += TokenType2Str(x);
            std::cout << ret << std::endl;
        }
    };

    struct ITEMS
    {
        int idx;
        std::set<Item> items;
        bool operator<(const ITEMS& rh) const
        {
            if (items.size() < rh.items.size())
                return true;
            else if (items.size() > rh.items.size())
                return false;
            else
            {
                if (items == rh.items)
                    return false;
                else
                {
                    auto it1 = items.begin();
                    auto it2 = rh.items.begin();
                    while (it1 !=items.end() && it2 != rh.items.end())
                    {
                        if (*it1 < *it2)
                            return true;
                        else if (*it2 < *it1)
                            return false;
                        else
                        {
                            ++it1;++it2;
                        }
                    }

                }
            }
            return false;
        }
    };

    struct ITEMS2ITEMS
    {
        ITEMS I;
        TokenType X;
        ITEMS J;
        bool operator<(const ITEMS2ITEMS& rh) const
        {
            if (I < rh.I)
                return true;
            else if (rh.I < I)
                return false;
            else
            {
                if (J < rh.J)
                    return true;
                else if (rh.J < J)
                    return false;
                else
                {
                    if (X < rh.X)
                        return true;
                    else if (X > rh.X)
                        return false;
                    else
                        return false;
                }
            }
        }
        bool operator==(const ITEMS2ITEMS& rh) const
        {
            return !(*this < rh) && !(rh < *this);
        }
    };

    bool FIND_RULE(TokenType T, std::vector<std::vector<TokenType >>& out);
    void FIRST(std::vector<TokenType > in, std::set<TokenType >& out)
    {
        auto T = in.front();
        std::vector<std::vector<TokenType >> tmp;
        if (FIND_RULE(T, tmp))
        {
            for (auto r : tmp)
            {
                std::vector<TokenType > aaa = std::vector<TokenType >(r.begin()+1, r.end());
                FIRST(aaa, out);
            }
        }
        else
        {
            out.insert(T);
        }
    }

    bool FIND_RULE(TokenType T, std::vector<std::vector<TokenType >>& out)
    {
        out.clear();
        for (auto r: rule)
        {
            if (r.front() == T)
            {
                out.push_back(r);
            }
        }
        return out.size() > 0;
    }

    // todo optimize efficiency
    void CLOSURE(std::set<Item>& I)
    {
        std::set<Item> news;
        size_t sz = 0;

        auto TT = I;
        do
        {
            news.clear();
            for (auto it : TT)
            {
                if (it.beta.size() <= 0) continue;
                auto X = it.beta.front();
                std::vector<std::vector<TokenType >> out;
                FIND_RULE(X, out);
                for (auto r : out)
                {
                    std::vector<TokenType > bz;
                    std::set<TokenType > ws;
                    bz = it.beta;
                    bz.erase(bz.begin());
                    bz.push_back(it.x);
                    FIRST(bz, ws);
                    for (auto w : ws)
                    {
                        Item im;
                        im.beta = std::vector<TokenType >(r.begin()+1, r.end());
                        im.A = X;
                        im.x = w;
                        news.insert(im);
                    }
                }
            }
            auto sz = I.size();
            I.insert(news.begin(), news.end());
            TT = news;
            if (sz == I.size()) break;
        }
        while (true);
        return;
    }

    void GOTO(std::set<Item> I, TokenType X, std::set<Item>& out)
    {
        std::set<Item> J;
        out.clear();
        for (auto it : I)
        {
            if (it.beta.size() > 0 && X == it.beta.front())
            {
                Item im;
                im.x = it.x;
                im.A = it.A;
                im.alpha = it.alpha;
                im.alpha.push_back(X);
                im.beta.insert(im.beta.end(), it.beta.begin()+1, it.beta.end());
                J.insert(im);
            }
        }
        CLOSURE(J);
        out = J;
    }

    void init_rule()
    {
        rule.resize(6);
        rule[0] = std::vector<TokenType >{ S1, S, TERMINAL };
        rule[1] = std::vector<TokenType >{ S, V, EQUAL, E };
        rule[2] = std::vector<TokenType >{ S, E};
        rule[3] = std::vector<TokenType >{ E, V};
        rule[4] = std::vector<TokenType >{ V, X};
        rule[5] = std::vector<TokenType >{ V, STAR, E};
    }
    /*void init_rule()
    {
        rule.resize(8);
        rule[0] = std::vector<TokenType >{ E1, E, TERMINAL };
        rule[1] = std::vector<TokenType >{ E, ID };
        rule[2] = std::vector<TokenType >{ E, NUM };
        rule[3] = std::vector<TokenType >{ E, E, STAR, E};
        rule[4] = std::vector<TokenType >{ E, E, CHU, E};
        rule[5] = std::vector<TokenType >{ E, E, PLUS, E};
        rule[6] = std::vector<TokenType >{ E, E, JIAN, E};
        rule[7] = std::vector<TokenType >{ E, LBRA, E, RBRA };
    }*/

    void G(std::set<Item> start, std::map<ITEMS, int>& mp, std::set<ITEMS2ITEMS>& EERET)
    {
        std::set<ITEMS> T;
        CLOSURE(start);
        ITEMS is;
        is.items = start;
        is.idx = 1;
        T.insert(is);

        std::set<ITEMS2ITEMS> EE;

        auto TT = T;
        do
        {
            std::set<ITEMS> news;
            for (auto it : TT)
            {
                for (auto item : it.items)
                {
                    if (item.beta.size() <= 0) continue;
                    auto X = item.beta.front();
                    if (X == TERMINAL) continue;
                    ITEMS J;
                    GOTO(it.items, X, J.items);
                    {
                        ITEMS2ITEMS i2i;
                        i2i.I = it;
                        i2i.X = X;
                        i2i.J = J;
                        EE.insert(i2i);
                    }
                    news.insert(J);
                }
            }
            auto sz = T.size();
            T.insert(news.begin(), news.end());
            TT = news;
            if (sz == T.size()) break;
        }
        while (true);

        // 状态集
        //std::map<ITEMS, int> mp;
        int idx = 0;
        std::set<ITEMS>::iterator it = T.begin();
        while (it != T.end())
        {
            auto is = *it;
            is.idx = idx;
            mp[is] = idx;
            ++it;++idx;
        }
        // 状态转换集 （边）
        //std::set<ITEMS2ITEMS> EERET;
        {

            auto it = EE.begin();
            while (it != EE.end())
            {
                auto i2i = *it;
                i2i.I.idx = mp[i2i.I];
                i2i.J.idx = mp[i2i.J];
                EERET.insert(i2i);
                ++it;
            }
        }
        return;
    }

    void GTbl()
    {
        init_rule();
        std::set<Item> start;
        Item im;
        {
            im.A = S1;
            im.beta.push_back(S);
            im.beta.push_back(TERMINAL);
            im.x = TERMINAL;
        }
        start.insert(im);
        CLOSURE(start);

        std::map<ITEMS, int> mp;
        std::set<ITEMS2ITEMS> EERET;
        G(start, mp, EERET);

        // 构建分析表[token][state]
        size_t states = mp.size();
        {
            tbl.resize(1 + TERMINAL);
            for (auto& it : tbl)
                it.resize(states);

            for (auto& it : tbl)
            {
                for (auto& itt : it)
                {
                    itt.state == ERR_STATE;
                }
            }
        }
        {

            // GN, SN
            for (auto it : EERET)
            {
                std::vector<std::vector<TokenType >> out;
                if (FIND_RULE(it.X, out))   // 非终结符
                {
                    action aa;
                    aa.act = GN;
                    aa.state = it.J.idx;
                    tbl[it.X][it.I.idx] = aa;
                }
                else
                {
                    action aa;
                    aa.act = SN;
                    aa.state = it.J.idx;
                    tbl[it.X][it.I.idx] = aa;
                }
            }
        }
        {
            // RK
            for (auto it : mp)
            {
                for (auto item : it.first.items)
                {
                    if (item.beta.size() == 1 && TERMINAL == item.beta[0])
                    {
                        action act;
                        act.act = AC;
                        act.state = 0;
                        tbl[TERMINAL][it.first.idx] = act;
                    }
                    if (item.beta.size() == 0)
                    {
                        std::vector<TokenType > tmp = item.alpha;
                        tmp.insert(tmp.begin(), item.A);
                        int idx = -1;
                        for (size_t i = 0; i < rule.size(); ++i)
                        {
                            if (rule[i] == tmp)
                            {
                                idx = i;
                                break;
                            }
                        }
                        if (idx >= 0)
                        {
                            if (tbl[item.x][it.first.idx].state == ERR_STATE)
                            {
                                action act;
                                act.act = RK;
                                act.state = idx;
                                tbl[item.x][it.first.idx] = act;
                            }
                            else
                            {
                                std::cout << "冲突\n";
                            }
                        }
                        else
                        {
                            std::cout << "err\n";
                        }
                    }
                }
            }
        }
        auto tmp = tbl;
        return;
    }

    void PTbl()
    {
        printf("%4s", " ");
        for (int token = 0; token <= TERMINAL; ++token)
        {
            //std::cout << "\t" << TokenType2Str((TokenType)token);
            printf("%4s  ", TokenType2Str((TokenType)token).c_str());
        }
        std::cout << std::endl;
        for (size_t state = 0; state < tbl[0].size(); ++state)
        {
            //std::cout << state << "\t";
            printf("%4d", state);
            for (int token = 0; token <= TERMINAL; ++token)
            {
                auto tmp = tbl;
                if (tbl[token][state].state != ERR_STATE)
                {
                    //std::cout << ActionType2Str(tbl[token][state].act) << tbl[token][state].state << "\t";
                    std::string st = std::to_string(tbl[token][state].state);
                    st = ActionType2Str(tbl[token][state].act) + st;
                    printf("%4s  ", st.c_str());
                }

                else
                    printf("%4s  ", " ");
                //std::cout << "     " << "\t";
            }
            std::cout << std::endl;
        }
    }

    void PRINT_ITEMS(ITEMS in)
    {
        std::cout << in.idx << ":" << std::endl;
        for (auto it : in.items)
        {
            it.print();
        }
    }

    void test()
    {
        init_rule();
        std::set<Item> start;
        Item im;
        {
            im.A = S1;
            im.beta.push_back(S);
            im.beta.push_back(TERMINAL);
            im.x = TERMINAL;
        }
        start.insert(im);
        CLOSURE(start);

        std::map<ITEMS, int> mp;
        std::set<ITEMS2ITEMS> EERET;
        G(start, mp, EERET);
        for (auto it : mp)
        {
            PRINT_ITEMS(it.first);
            std::cout << std::endl;
        }

        for (auto it : EERET)
        {
            std::cout << it.I.idx << " " << TokenType2Str(it.X) << " " << it.J.idx << std::endl;
        }
        return;
    }
    /*void test()
    {
        init_rule();
        std::set<Item> start;
        Item im;
        {
            im.A = E1;
            im.beta.push_back(E);
            im.beta.push_back(TERMINAL);
            im.x = TERMINAL;
        }
        start.insert(im);        CLOSURE(start);


        std::map<ITEMS, int> mp;
        std::set<ITEMS2ITEMS> EERET;
        G(start, mp, EERET);
        for (auto it : mp)
        {
            PRINT_ITEMS(it.first);
            std::cout << std::endl;
        }

        for (auto it : EERET)
        {
            std::cout << it.I.idx << " " << TokenType2Str(it.X) << " " << it.J.idx << std::endl;
        }
        return;
    }*/
}

namespace lex
{
    const int EPSILON = 200;
    const int NUM_CHAR = 256;
    std::vector<int> NFA[20][NUM_CHAR];
    void INIT_NFA()
    {

        {
            NFA[1][EPSILON].push_back(4);
            NFA[1][EPSILON].push_back(9);
            NFA[1][EPSILON].push_back(14);
            NFA[1]['i'].push_back(2);
        }
        {
            NFA[2]['f'].push_back(3);
        }
        {
            // 3
        }
        {
            for (char c = 'a'; c <= 'z'; ++c)
            {
                NFA[4][c].push_back(5);
            }
        }
        {
            NFA[5][EPSILON].push_back(8);
        }
        {
            for (char c = 'a'; c <= 'z'; ++c)
            {
                NFA[6][c].push_back(7);
            }
            for (char c = '0'; c <= '9'; ++c)
            {
                NFA[6][c].push_back(7);
            }
        }
        {
            NFA[7][EPSILON].push_back(8);
        }
        {
            NFA[8][EPSILON].push_back(6);
        }
        {
            for (char c = '0'; c <= '9'; ++c)
            {
                NFA[9][c].push_back(10);
            }
        }
        {
            NFA[10][EPSILON].push_back(13);
        }
        {
            for (char c = '0'; c <= '9'; ++c)
            {
                NFA[11][c].push_back(12);
            }
        }
        {
            NFA[12][EPSILON].push_back(13);
        }
        {
            NFA[13][EPSILON].push_back(11);
        }
        {
            // 任何字符
            for (size_t i = 0; i < NUM_CHAR; ++i)
            {
                NFA[14][i].push_back(15);
            }
            NFA[14][EPSILON].clear();
        }
    }

    void CLOSURE(std::set<int>& S)
    {
        std::set<int> TT = S;
        do
        {
            std::set<int> news;
            for (auto state: TT)
            {
                auto ss = NFA[state][EPSILON];
                for (auto sta : ss)
                    news.insert(sta);
            }
            auto szori = S.size();
            S.insert(news.begin(), news.end());
            TT = news;
            if (S.size() == szori) break;
        }
        while (true);
        return;
    }

    void DFA_EDGE(const std::set<int>& d, char c, std::set<int>& out)
    {
        out.clear();
        for (auto state : d)
        {
            auto ss = NFA[state][c];
            out.insert(ss.begin(), ss.end());
        }
        CLOSURE(out);
        return;
    }

    struct CC
    {
        std::set<int> I;
        char c;
        std::set<int> J;
    };

    void test()
    {
        INIT_NFA();
        std::set<int> S = { 1 };
        CLOSURE(S);

        std::set<std::set<int>> DFA;
        DFA.insert(S);

        std::vector<CC> ccs;

        auto TT = DFA;
        do
        {
            std::set<std::set<int>> news;
            for (auto ss : TT)
            {
                std::set<int> out;
                for (int i = 0; i < NUM_CHAR; ++i)
                {
                    if (i != EPSILON)
                    {
                        DFA_EDGE(ss, (unsigned char)i, out);
                        {
                            if (out.size() > 0)
                            {
                                CC c;
                                c.I = ss; c.c = i; c.J = out;
                                ccs.push_back(c);
                                news.insert(out);
                            }
                        }
                    }
                }
            }
            auto szori = DFA.size();
            DFA.insert(news.begin(), news.end());
            TT = news;
            if (szori == DFA.size()) break;
        }
        while (true);

        // 整理
        std::map<std::set<int>, int> mp;
        std::map<int, std::set<int>> mp1;
        int DFA_[20][NUM_CHAR];
        {
            int count = 1;
            for (auto& it : DFA)
            {
                mp[it] = count++;
                mp1[mp[it]] = it;
            }
        }
        {

            for (auto& it : ccs)
            {
                DFA_[mp[it.I]][it.c] = mp[it.J];
            }
        }

        auto k = DFA_[mp[S]]['5'];
        auto sss = mp1[k];

        return;
    }
}

int main() {
    //init_action_tbl();
    //init_rule();
    //std::vector<TokenType > input = { ID, EQUAL, NUM, SEMICOLON, ID, EQUAL, ID, PLUS, LBRACKET, ID, EQUAL, NUM, PLUS, NUM, COMMA, ID, RBRACKET, TERMINAL };
    //parse(input);
    test::test();
    test::GTbl();
    test::PTbl();
    lex::test();
    return 0;
}