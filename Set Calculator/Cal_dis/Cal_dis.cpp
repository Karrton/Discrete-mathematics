#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <ctime> 

using namespace std;

class Set 
{
     vector<int> elements;
     int low;
     int upp;
public:
    Set() {}

    void add(int element) 
    {
        if (find(elements.begin(), elements.end(), element) == elements.end()) 
        {
            elements.push_back(element);
            sort(elements.begin(), elements.end());
        }
    }

    Set unionWith(const Set& other) const 
    {
        Set result = *this;
        for (int element : other.elements) 
        {
            result.add(element);
        }
        return result;
    }

    Set intersectionWith(const Set& other) const 
    {
        Set result;
        for (int element : elements) 
        {
            if ( find(other.elements.begin(), other.elements.end(), element) != other.elements.end()) 
            {
                result.add(element);
            }
        }
        return result;
    }

    Set differenceWith(const Set& other) const 
    {
        Set result;
        for (int element : elements) 
        {
            if ( find(other.elements.begin(), other.elements.end(), element) == other.elements.end()) 
            {
                result.add(element);
            }
        }
        return result;
    }

    Set symmetricDifferenceWith(const Set& other) const 
    {
        return this->differenceWith(other).unionWith(other.differenceWith(*this));
    }

    size_t getSize() const 
    {
        return elements.size();
    }

    int getElement(size_t index) const 
    {
        if (index < elements.size())
        {
            return elements[index];
        }
        else 
        {
            throw  out_of_range("Index out of range");
        }
    }

    void setUniverse(int lower_bound, int upper_bound) 
    {
        low = lower_bound;
        upp = upper_bound;
    }

    Set complement(int lower_bound, int upper_bound) const 
    {
        Set result;
        for (int i = lower_bound; i <= upper_bound; i++) 
        {
            if ( find(elements.begin(), elements.end(), i) == elements.end()) 
            {
                result.add(i);
            }
        }
        return result;
    }

    Set generateRandomSorted(int count)
    {
        Set result;

        for (int i = 0; i < count; ++i)
        {
            int randomNum = rand() % (upp - low + 1) + low;
            result.add(randomNum);
        }

        sort(result.elements.begin(), result.elements.end());
        return result;
    }

    Set generateUniqueEvens(int n) const
    {
        if (n > (upp - low + 1) / 2)
        {
            throw invalid_argument("Нельзя сгенерировать столько уникальных чисел, повторите ввод.");
        }

        Set result;
        vector<int> possibleValues;

        for (int i = low % 2 == 0 ? low : low + 1; i <= upp; i += 2)
        {
            possibleValues.push_back(i);
        }

        random_shuffle(possibleValues.begin(), possibleValues.end());

        for (int i = 0; i < n; ++i)
        {
            result.add(possibleValues[i]);
        }

        return result;
    }

    Set generateUniqueOdds(int n) const
    {
        if (n > (upp - low + 1 + low % 2) / 2)
        {
            throw invalid_argument("Нельзя сгенировать столько уникальных чисел, повторите ввод.");
        }

        Set result;
        int current = low % 2 != 0 ? low : low + 1;

        std::vector<int> candidates;

        for (int i = low; i <= upp; i += 2)
        {
            candidates.push_back(i);
        }

        for (int i = 0; i < n; ++i)
        {
            if (candidates.empty())
            {
                throw invalid_argument("Невозможно сгенерировать достаточно уникальных чисел.");
            }

            int randomIndex = rand() % candidates.size();
            result.add(candidates[randomIndex]);
            candidates.erase(candidates.begin() + randomIndex);
        }

        return result;
    }

    Set generateMultiples(int n, int multiple) const
    {
        int possibleCount = (upp - low + 1) / multiple;

        if (n > possibleCount)
        {
            throw std::invalid_argument("Нельзя сгенерировать n чисел в количестве " + std::to_string(n) + " кратных " + std::to_string(multiple) + " в заданном диапазоне.");
        }

        Set result;
        int rangeSize = upp - low + 1;

        while (result.getSize() < n)
        {
            int randomNum = rand() % rangeSize + low; // Генерация случайного числа в заданном диапазоне
            if (randomNum % multiple == 0)
            {
                result.add(randomNum);
            }
        }

        return result;
    }

    Set& operator=(const Set& other)
    {
        if (this == &other)
            return *this;

        elements = other.elements;
        low = other.low;
        upp = other.upp;

        return *this;
    }

    Set(const Set& other)
    {
        elements = other.elements;
        low = other.low;
        upp = other.upp;
    }

    string toString() const
    {
        string result = "{";

        for (size_t i = 0; i < elements.size(); ++i)
        {
            result += to_string(elements[i]);
            if (i != elements.size() - 1)
                result += ',';
        }

        result += "}";
        return result;
    }
};

string process_complement(string expression, int lower_bound, int upper_bound) 
{
    size_t complement_pos = expression.find('c');

    while (complement_pos != string::npos) 
    {
        int open_bracket = expression.find('{', complement_pos);
        int close_bracket = expression.find('}', open_bracket);

        // Извлекаем подвыражение, для которого нужно найти дополнение
        string sub_expression = expression.substr(open_bracket, close_bracket - open_bracket + 1);
        Set set_to_complement;

        // Преобразуем подвыражение в Set
        for (int i = 1; i < sub_expression.size(); i++) {
            if (sub_expression[i] > 47 && sub_expression[i] < 58) {
                set_to_complement.add(sub_expression[i] - '0');
            }
        }

        // Находим дополнение
        Set complement;
        complement.setUniverse(lower_bound, upper_bound);
        complement = set_to_complement.complement(lower_bound, upper_bound);

        // Преобразуем дополнение обратно в строку
        string complement_string = "{";
        for (int i = 0; i < complement.getSize(); i++) {
            complement_string += to_string(complement.getElement(i));
            if (i != complement.getSize() - 1) complement_string += ',';
        }
        complement_string += "}";

        // Заменяем подвыражение на дополнение
        expression.replace(open_bracket, close_bracket - open_bracket + 1, complement_string);

        // Удаляем символ 'c'
        expression.erase(complement_pos, 1);

        // Ищем следующее вхождение операции дополнения
        complement_pos = expression.find('c');
    }

    return expression;
}

string counting(string val, pair<int, int> staples, int lower_bound, int upper_bound)
{
    int open = staples.first;
    int close = staples.second; 
    string buff, other, expression;
    bool isEnd = false;

    // Извлекаем выражение
    if (val.find('(') < val.size()) expression = val.substr(open + 1, close - open - 1);
    else expression = val.substr(open, close - open + 1);

    expression = process_complement(expression, 0, 15);

    while (true)
    {
        buff.clear();
        Set A, B;
        size_t Union = expression.find('v');
        size_t Intersection = expression.find('n');
        size_t Difference = expression.find('\\');
        size_t Symmetric_difference = expression.find('^');
        size_t first_op_pos = min({ Union, Intersection, Difference, Symmetric_difference });

        if (first_op_pos != string::npos)
        {
            // Разделяем выражение на две части - до и после операции
            string left_operand = expression.substr(0, first_op_pos);
            string right_operand = expression.substr(first_op_pos + 1, expression.find('}', first_op_pos + 1) - first_op_pos);
            other = expression.substr(expression.find('}', first_op_pos + 1) + 1);

            // Преобразуем string в Set
            for (int i = 1; i < left_operand.size(); i++)
            {
                if (left_operand.length() == 2) break;
                if (left_operand[i] > 47 && left_operand[i] < 58 || left_operand[i] == '-') { buff += left_operand[i]; }
                else 
                { 
                    if (buff[0] == '-')
                    {
                        buff.erase(0, 1);
                        int numBuff = stoi(buff);
                        numBuff *= -1;
                        A.add(numBuff);
                        buff.clear();
                    }
                    else A.add(stoi(buff)); buff.clear(); 
                }
            }

            for (int i = 1; i < right_operand.size(); i++)
            {
                if (right_operand.length() == 2) break;
                if (right_operand[i] > 47 && right_operand[i] < 58 || right_operand[i] == '-') { buff += right_operand[i]; }
                else 
                { 
                    if (buff[0] == '-')
                    {
                        buff.erase(0, 1);
                        int numBuff = stoi(buff);
                        numBuff *= -1;
                        A.add(numBuff);
                        buff.clear();
                    }
                    else B.add(stoi(buff)); buff.clear(); 
                }
            }

            if (first_op_pos == Union) { A = A.unionWith(B); }
            else if (first_op_pos == Intersection) { A = A.intersectionWith(B); }
            else if (first_op_pos == Difference) { A = A.differenceWith(B); }
            else if (first_op_pos == Symmetric_difference) { A = A.symmetricDifferenceWith(B); }
            else { cout << "Error" << endl; }
        }

        else break;

        buff.clear();
        buff += '{';
        for (int i = 0; i < A.getSize(); i++) { buff += to_string(A.getElement(i)); if (i != A.getSize() - 1) buff += ','; } 
        buff += '}';
        buff += other;
        expression = buff;
        other.clear();
    }

    for (int i = 0; i < staples.first; i++)  buff += val[i];
    buff += expression;
    for (int i = ++staples.second; i < val.size(); i++)  buff += val[i];

    return buff;
}

pair<int, int> find_staples(string val)
{
	int open = -1; int close = -1;
	for (int i = 0; i < val.size(); i++)
	{
		if (val[i] == '(') { open = i; }
		else if (val[i] == ')') { close = i; return make_pair(open, close); }
	}
}

pair<int, int> find_expression(string val)
{
    int close = -1;
    close = val.find('}');
    close = val.find('}', ++close);
    if (close != -1) return make_pair(0, close);
    else return make_pair(-1, -1);
}

string closeStaples(bool& isStaples)
{
    if (isStaples)
    {
        do
        {
            int a;
            cout << "Хотие закрыть скобки?\n1.Да\nЛюбое число.Нет" << endl;
            cin >> a;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка: Введите корректное число." << endl;
                continue;
            }
            if (a == 1)
            {
                isStaples = false;
                return ")";
            }
            else
            {
                break;
            }
        } while (isStaples);
    }
    return "";
}

string chooseStaples(bool &isStaples)
{
    if (!isStaples)
    {
        do
        {
            int a;
            system("cls");
            cout << "Хотие открыть скобки?\n1.Да\nЛюбое число.Нет" << endl;
            cin >> a;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка: Введите корректное число." << endl;
                continue;
            }
            if (a == 1)
            {
                isStaples = true;
                return "(";
            }
            else
            {
                break;
            }
        } while (!isStaples);
    }
    return "";
}

string chooseSign(bool &isPut)
{
    string buff[4] = {"v", "n", "\\", "^"};
    string userInput;
    bool isRight = true;
    cin.get();
;   do
    {
        system("cls");
        if (!isRight)
            cout << "Ошибка: Введите знак из списка ниже." << endl;
        cout << "Объединение: v\nПересечение n\nРазность: \\\nСимметрическая разность: ^\n" << endl;
        cout << "Введите знак: " << endl;
        getline(cin, userInput);
        for (int i = 0; i < 4; i++)
        {
            if (userInput == buff[i])
                isRight = false;
        }
    } while (isRight);
    isPut = false;
    return userInput;
}

string userInputUniqueNumbers(int a, int b)
{
    vector<bool> isUsed(b - a + 1, false); // Массив, показывающий, использовано ли число
    vector<int> uniqueNumbers; // Вектор для хранения уникальных чисел

    int num;
    while (true)
    {
        system("cls");
        cout << "Введите уникальное число в диапазоне [" << a << ", " << b << "], или 0 для завершения: ";
        cin >> num;

        if (num == 0)
            break;

        if (num < a || num > b)
        {
            cout << "Число вне диапазона." << endl;
            system("pause");
            continue;
        }

        if (isUsed[num - a])
        {
            cout << "Число уже использовано." << endl;
            system("pause");
            continue;
        }

        uniqueNumbers.push_back(num);
        isUsed[num - a] = true;
    }

    string result = "{";
    for (size_t i = 0; i < uniqueNumbers.size(); ++i)
    {
        result += to_string(uniqueNumbers[i]);
        if (i != uniqueNumbers.size() - 1)
            result += ',';
    }
    result += "}";

    return result;
}

string chooseComplement()
{
    int c;
    do
    {
        system("cls");
        cout << "Хотите добавить дополнение?" << endl;
        cout << "1.Да\nЛюбое число.Нет" << endl;
        cin >> c;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: Введите корректное число." << endl;
            continue;
        }
        if (c == 1)
        {
            return "c";
        }
        else break;
    } while (true);
    return "";
}

int main()
{
    setlocale(LC_ALL, "Rus");

    int a, b;
    bool sign = false;
    bool isStaples = false;
    bool isClose = false;
    int numStaples = 0;

    while (true)
    {
        system("cls");
        cout << "Введите нижнюю границу универсума: " << endl;
        cin >> a;
        cout << "Введите верхнюю границу универсума: " << endl;
        cin >> b;

        if (cin.fail())
        {
            system("cls");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: Введите корректный диапазон." << endl;
            system("pause");
            continue;
        }

        if (a <= b)
            break;

        system("cls");
        cout << "Ошибка: Начальное значение диапазона должно быть меньше или равно конечному." << endl;
        system("pause");
    }

    string exp;

    while (true)
    {
        system("cls");
        if (!exp.empty())
            cout << "Текущее выражение: " << exp << endl;

        cout << "1. Ввести множество вручную" << endl;
        cout << "2. Сгенерировать множество" << endl;
        cout << "Другая цифра или символ - завершить ввод" << endl;

        int choose;
        cin >> choose;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: Введите корректное число." << endl;
            continue;
        }

        Set set_a;
        set_a.setUniverse(a, b);

        switch (choose)
        {
        case 1:
            if (sign)
            {
                exp += chooseSign(sign);
            }
            system("cls");
            exp += chooseComplement();
            exp += chooseStaples(isStaples);
            exp += userInputUniqueNumbers(a, b);
            sign = true;
            if (isStaples) ++numStaples;
            if (isStaples && numStaples >= 2)
            {
                string str = closeStaples(isStaples);
                if (str != "")
                {
                    exp += str;
                    numStaples = 0;
                }
            }
            break;
        case 2:
            if (sign)
            {
                exp += chooseSign(sign);
            }
            exp += chooseComplement();
            exp += chooseStaples(isStaples);
            if (isStaples) ++numStaples;
            system("cls");
            cout << "1. Сгенерировать чётное множество" << endl;
            cout << "2. Сгенерировать нечётное множество" << endl;
            cout << "3. Сгенерировать кратное множество" << endl;
            cout << "4. Сгенерировать случайное множество" << endl;

            int s;
            cin >> s;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка: Введите корректное число." << endl;
                system("pause");
                continue;
            }

            if (s == 1)
            {
                do
                {
                    system("cls");
                    cout << "Введите количество генерируемых чисел: ";
                    int n;
                    cin >> n;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ошибка: Введите корректное число." << endl;
                        system("pause");
                        continue;
                    }

                    try
                    {
                        exp += set_a.generateUniqueEvens(n).toString();
                        sign = true;
                    }

                    catch (const std::invalid_argument& e)
                    {
                        system("cls");
                        cout << "Ошибка: " << e.what() << std::endl;
                        system("pause");
                        sign = false;
                    }
                } while (!sign);
            }
            else if (s == 2)
            {
                do
                {
                    system("cls");
                    cout << "Введите количество генерируемых чисел: ";
                    int n;
                    cin >> n;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ошибка: Введите корректное число." << endl;
                        system("pause");
                        continue;
                    }

                    try
                    {
                        exp += set_a.generateUniqueOdds(n).toString();
                        sign = true;
                    }

                    catch (const std::invalid_argument& e)
                    {
                        system("cls");
                        cout << "Ошибка: " << e.what() << std::endl;
                        system("pause");
                        sign = false;
                    }
                } while (!sign);
            }
            else if (s == 3)
            {
                do
                {
                    system("cls");
                    cout << "Введите количество генерируемых чисел: ";
                    int n;
                    cin >> n;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ошибка: Введите корректное число." << endl;
                        system("pause");
                        continue;
                    }

                    system("cls");
                    cout << "Введите число, которому будут кратны генерируемые числа: ";
                    int m;
                    cin >> m;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ошибка: Введите корректное число." << endl;
                        system("pause");
                        continue;
                    }

                    try
                    {
                        exp += set_a.generateMultiples(n, m).toString();
                        sign = true;
                    }

                    catch (const std::invalid_argument& e)
                    {
                        system("cls");
                        cout << "Ошибка: " << e.what() << std::endl;
                        system("pause");
                        sign = false;
                    }
                } while (!sign);
            }
            else if (s == 4)
            {
                do
                {
                    system("cls");
                    cout << "Введите количество генерируемых чисел: ";
                    int n;
                    cin >> n;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ошибка: Введите корректное число." << endl;
                        system("pause");
                        continue;
                    }

                    try
                    {
                        exp += set_a.generateRandomSorted(n).toString();
                        sign = true;
                    }

                    catch (const std::invalid_argument& e)
                    {
                        system("cls");
                        cout << "Ошибка: " << e.what() << std::endl;
                        system("pause");
                        sign = false;
                    }
                } while (!sign);
                }
            if (isStaples && numStaples >= 2)
            {
                string str = closeStaples(isStaples);
                if (str != "")
                {
                    exp += str;
                    numStaples = 0;
                }
            }
            break;
        default:
            if(!isStaples)
            {
                //exp = "You can enter your example here if you don't want to enter it through the console. After filling in this field, immediately call the expression count.";
                system("cls");
                string buff3;
                cout << "Решение:" << endl;
                cout << exp << endl;
                pair<int, int> staples;
                while (exp.find('(') != -1)
                {
                    staples = find_staples(exp);
                    exp = counting(exp, staples, a, b);
                    cout << exp << endl;
                }
                exp = process_complement(exp, a, b);
                cout << exp << endl;
                while (find_expression(exp).second != -1)
                {
                    staples = find_expression(exp);
                    exp = counting(exp, find_expression(exp), a, b);
                    cout << exp << endl;
                }
                return 0;
            }
	    else
	    {
		system("cls");
		cout << "Ошибка: Скобки не закрыты." << endl;
		system("pause");
		break;    
	    }
        }
    }
    return 0;
}
