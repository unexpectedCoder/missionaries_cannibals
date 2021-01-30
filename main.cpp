#include <iostream>
#include <string>
#include <vector>


struct State
{
    int **data;
    int n;
    int rule;
    int shore;
    bool terminal;
    State *next;

    explicit State() :
        data(new int*[2]), n(2), rule(0), shore(0), terminal(false), next(nullptr)
    {
        for (int i = 0; i < n; ++i)
            data[i] = new int[n];
        data[0][0] = 3;
        data[0][1] = 3;
        data[1][0] = 0;
        data[1][1] = 0;
    }
    State(const State &s) :
        data(new int*[2]), n(2), rule(0), shore(s.shore), terminal(s.terminal), next(nullptr)
    {
        for (int i = 0; i < n; ++i) {
            data[i] = new int[n];
            for (int j = 0; j < n; ++j)
                data[i][j] = s.data[i][j];
        }
    }
    ~State()
    {
        if (next) {
            delete next;
            next = nullptr;
        }

        if (data) {
            for (int i = 0; i < n; ++i)
                delete[] data[i];
            delete[] data;
        }
    }
    friend bool operator==(const State &s1, const State &s2);
    friend std::ostream& operator<<(std::ostream&, const State&);
};


bool operator==(const State &s1, const State &s2)
{
    if (&s1 == &s2)
        return true;
    for (int i = 0; i < s1.n; ++i)
        for (int j = 0; j < s1.n; ++j)
            if (s1.data[i][j] != s2.data[i][j])
                return false;
    return s1.shore == s2.shore;
}


std::ostream& operator<<(std::ostream &os, const State &s)
{
    os << "Состояние:"
       << " левый берег: [" << s.data[0][0] << "; " << s.data[0][1] << "],"
       << " правый берег: [" << s.data[1][0] << "; " << s.data[1][1] << "],"
       << " лодка у берега #" << s.shore + 1;
    return os;
}


typedef std::vector<State> States;
typedef std::vector<State>::const_iterator States_citr;


int getOtherShore(int shore);
bool isRule_1(const State *s);
State* makeRule_1(const State *s);
bool isRule_2(const State *s);
State* makeRule_2(const State *s);
bool isRule_3(const State *s);
State* makeRule_3(const State *s);
bool isRule_4(const State *s);
State* makeRule_4(const State *s);
bool isRule_5(const State *s);
State* makeRule_5(const State *s);
void solve(State *s, States &story, int rule, int rules);
bool checkIn(const States &story, const State *s);
bool isTerminal(const State*);
void printResult(State*);
std::string getRuleText(int rule);


int main()
{
    using namespace std;

    cout << "\t *** Миссионеры и Людоеды ***\n\n";
    
    // Начальное состояние
    int rule = 0;
    int rules = 5;
    State *state = new State;
    States story;
    story.push_back(*state);

    // Контроль начального состояния
    cout << "Начальное состояние:\n";
    cout << *state << endl;

    // Моделирование
    cout << "Идёт моделирование...\n";
    solve(state, story, rule, rules);

    // Результаты
    cout << "Решение:\n";
    printResult(state);
    
    delete state;
    return 0;
}


void solve(State *s, States &story, int rule, int rules)
{
    if (isTerminal(s)) {
        s->terminal = true;
        return;
    }

    State *ns = nullptr;
    for (int i = 1; i <= rules; ++i) {
        if (ns) {
            delete ns;
            ns = nullptr;
        }
        if (i == rule)
            continue;

        switch (i) {
        case 1: 
            if (isRule_1(s))
                ns = makeRule_1(s);
            break;
        case 2:
            if (isRule_2(s))
                ns = makeRule_2(s);
            break;
        case 3:
            if (isRule_3(s))
                ns = makeRule_3(s);
            break;
        case 4:
            if (isRule_4(s))
                ns = makeRule_4(s);
            break;
        case 5:
            if (isRule_5(s))
                ns = makeRule_5(s);
            break;
        default:
            ns = nullptr;
        }
        
        if (ns) {
            s->next = ns;
            
            if (checkIn(story, ns))
                continue;
            story.push_back(*ns);

            solve(ns, story, i, rules);
            
            if (ns->terminal) {
                s->rule = i;
                s->next = ns;
                s->terminal = true;
                return;
            }
        }
    }
}


bool checkIn(const States &story, const State *s)
{
    for (States_citr itr = story.begin(); itr != story.end(); ++itr)
        if (*itr == *s)
            return true;
    return false;
}


bool isRule_1(const State *s)
{
    if (s->data[s->shore][0] == 0) // нет миссионеров
        return false;
    if (s->data[s->shore][1] > s->data[s->shore][0] - 1 and s->data[s->shore][0] - 1 != 0)
        return false;
    int otherShore = getOtherShore(s->shore);
    if (s->data[otherShore][1] > s->data[otherShore][0] + 1)
        return false;
    return true;
}


State* makeRule_1(const State *s)
{
    State *ns = new State(*s);
    ns->data[s->shore][0]--;
    ns->shore = getOtherShore(s->shore);
    ns->data[ns->shore][0]++;
    return ns;
}


bool isRule_2(const State *s)
{
    if (s->data[s->shore][1] == 0) // нет людоедов
        return false;
    int otherShore = getOtherShore(s->shore);
    if (s->data[otherShore][1] + 1 > s->data[otherShore][0] and s->data[otherShore][0] != 0)
        return false;
    return true; 
}


State* makeRule_2(const State *s)
{
    State *ns = new State(*s);
    ns->data[s->shore][1]--;
    ns->shore = getOtherShore(s->shore);
    ns->data[ns->shore][1]++;
    return ns;
}


bool isRule_3(const State *s)
{
    if (s->data[s->shore][0] == 0 or s->data[s->shore][1] == 0) // нет нужного кол-ва людей
        return false;
    if (s->data[s->shore][1] - 1 > s->data[s->shore][0] - 1 and s->data[s->shore][0] - 1 != 0)
        return false;
    int otherShore = getOtherShore(s->shore);
    if (s->data[otherShore][1] + 1 > s->data[otherShore][0] + 1)
        return false;
    return true;
}


State* makeRule_3(const State *s)
{
    State *ns = new State(*s);
    ns->data[s->shore][0]--;
    ns->data[s->shore][1]--;
    ns->shore = getOtherShore(s->shore);
    ns->data[ns->shore][0]++;
    ns->data[ns->shore][1]++;
    return ns;
}


bool isRule_4(const State *s)
{
    if (s->data[s->shore][0] < 2) // нет стольких миссионеров
        return false;
    if (s->data[s->shore][1] > s->data[s->shore][0] - 2 and s->data[s->shore][0] - 2 != 0)
        return false;
    int otherShore = getOtherShore(s->shore);
    if (s->data[otherShore][1] > s->data[otherShore][0] + 2)
        return false;
    return true;
}


State* makeRule_4(const State *s)
{
    State *ns = new State(*s);
    ns->data[s->shore][0] -= 2;
    ns->shore = getOtherShore(s->shore);
    ns->data[ns->shore][0] += 2;
    return ns;
}


bool isRule_5(const State *s)
{
    if (s->data[s->shore][1] < 2) // нет стольких людоедов
        return false;
    if (s->data[s->shore][1] - 2 > s->data[s->shore][0] and s->data[s->shore][0] != 0)
        return false;
    int otherShore = getOtherShore(s->shore);
    if (s->data[otherShore][1] + 2 > s->data[otherShore][0] and s->data[otherShore][0] != 0)
        return false;
    return true;
}


State* makeRule_5(const State *s)
{
    State *ns = new State(*s);
    ns->data[s->shore][1] -= 2;
    ns->shore = getOtherShore(s->shore);
    ns->data[ns->shore][1] += 2;
    return ns;
}


int getOtherShore(int shore)
{
    // Поменять номер берега
    return shore ? 0 : 1;
}


bool isTerminal(const State *s)
{
    return s->data[1][0] == 3 and s->data[1][1] == 3;
}


void printResult(State *s)
{
    using namespace std;
    
    string arrows[] = {"->", "<-"};
    State *p = s;
    int i = 0;
    while (p->next) {
        cout << " Шаг #" << ++i << " \t" << arrows[p->shore] << ": " << getRuleText(p->rule) << " ==> " << *(p->next) << endl;
        p = p->next;
    }
}


std::string getRuleText(int rule)
{
    if (rule == 0) return "ничего не делать";
    if (rule == 1) return "(П1) переправить миссионера";
    if (rule == 2) return "(П2) переправить людоеда";
    if (rule == 3) return "(П3) переправить миссионера и людоеда";
    if (rule == 4) return "(П4) переправить 2-х миссионеров";
    if (rule == 5) return "(П5) переправить 2-х людоедов";
    return "ОШИБКА! Неизвестное правило!";
}

