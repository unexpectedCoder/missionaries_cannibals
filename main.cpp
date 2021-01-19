#include <iostream>


struct State
{
    int **data;
    int n;
    State *prev;
    int rule;

    State(State *parent=nullptr) : data(new int*[2]), n(2), prev(parent), rule(0)
    {
        for (int i = 0; i < n; ++i) {
            data[i] = new int[n];
            for (int j = 0; j < n; ++j)
                data[i][j] = 0;
        }
    }
    ~State()
    {
        for (int i = 0; i < n; ++i)
            delete[] data[i];
        delete[] data;
    }
};


int get_other_shore(int shore);
void make_copy(const State &state, State &new_state);
bool is_rule_1(const State &s, int shore);
bool is_rule_2(const State &s, int shore);
bool is_rule_3(const State &s, int shore);
bool is_rule_4(const State &s, int shore);
bool is_rule_5(const State &s, int shore);
bool is_terminal(const State &state, int n);
void print_state(const State &s);


int main()
{
    using namespace std;

    // Начальное состояние
    int rules = 5;
    int humans = 3;             // кол-во человек с каждой стороны
    State state;                // переменная состояния системы
    state.data[0][0] = humans;
    state.data[0][1] = humans;
    int boat[state.n] = {0};    // состояние лодки
    int shore = 0;              // текущий берег (номер строки матрицы state)
    // Контроль начального состояния    
    cout << "Начальное состояние\n";
    print_state(state);
    cout << "Boat: [" << boat[0] << "; " << boat[1] << "]\n";
    cout << "Shore: " << shore << '\n';

    // Моделирование
    // ...прямое прохождение
    for (int i = 1; i <= rules; ++i) {
        State new_state(&state);
        new_state.rule = i;
    }

    return 0;
}


int get_other_shore(int shore)
{
    // Поменять номер берега
    if (shore)
        return 0;
    return 1;
}


void make_copy(const State &state, State &new_state)
{
    new_state.n = state.n;
    new_state.prev = state.prev;
    new_state.rule = state.rule;
    for (int i = 0; i < state.n; ++i)
        for (int j = 0; j < state.n; ++j)
            new_state.data[i][j] = state.data[i][j];
}


bool is_rule_1(const State &s, int shore)
{
    if (s.data[shore][0] == 0) // нет миссионеров
        return false;
    if (s.data[shore][0] - 1 > 0 and s.data[shore][0] - 1 < s.data[shore][1])
        return false;
    return true;
}


bool is_rule_2(const State &s, int shore)
{
    if (s.data[shore][1] == 0) // нет людоедов
        return false;
    int other_shore = get_other_shore(shore);
    if (s.data[other_shore][0] > 0 and s.data[other_shore][1] + 1 > s.data[other_shore][0])
        return false;
    return true; 
}


bool is_rule_3(const State &s, int shore)
{
    if (s.data[shore][0] == 0 or s.data[shore][1] == 0) // нет нужного кол-ва людей
        return false;
    int other_shore = get_other_shore(shore);
    if (s.data[other_shore][1] + 1 > s.data[other_shore][0] + 1)
        return false;
    return true;
}


bool is_rule_4(const State &s, int shore)
{
    if (s.data[shore][0] < 2) // нет стольких миссионеров
        return false;
    if (s.data[shore][0] - 2 < s.data[shore][1])
        return false;
    return true;
}


bool is_rule_5(const State &s, int shore)
{
    if (s.data[shore][1] < 2) // нет стольких людоедов
        return false;
    int other_shore = get_other_shore(shore);
    if (s.data[other_shore][0] > 0 and s.data[other_shore][1] + 2 > s.data[other_shore][0])
        return false;
    return true;
}


bool is_terminal(const State &state, int n)
{
    if (state.data[1][0] == n and state.data[1][1] == state.n)
        return true;
    return false;
}


void print_state(const State &s)
{
    using namespace std;

    cout << "State has n=" << s.n << " rule=" << s.rule << ":\n";
    cout << " - left shore: [" << s.data[0][0] << "; " << s.data[0][1] << "]\n"
         << " - right shore: [" << s.data[1][0] << "; " << s.data[1][1] << "]\n";
    if (s.prev)
        cout << " - has previous state\n";
}

