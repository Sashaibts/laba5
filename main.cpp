#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates (2).h"

using namespace std;

enum class SportDanger : int
{
    Safely,
    Medium,
    Dangerous,
    Unknown
};
// Реализация паттерна "Стратегия"

enum class SportMannerEnum : int
{
  Swallow,
  BiteOff,
  Chop,
  None
};

class SportStrategy
{
public:
  virtual ~SportStrategy() {}
  virtual void Competition() = 0;
};

class SwallowSportStrategy : public SportStrategy
{
  void Competition() { cout << "Swallow the whole sport..."; }
};

class BiteOffSportStrategy : public SportStrategy
{
  void Competition() { cout << "Bite off a piece..."; }
};

class ChopSportStrategy : public SportStrategy
{
  void Competition() { cout << "Chop into little pieces..."; }
};

class GrindStrategy : public SportStrategy
{
  void Competition() { cout << "Grind using blender..."; }
};

// Фабричный метод для создания стратегий
SportStrategy *CreateSportStrategy(SportMannerEnum sportManner)
{
  switch(sportManner)
  {
    case SportMannerEnum::Swallow: return new SwallowSportStrategy;
    case SportMannerEnum::BiteOff: return new BiteOffSportStrategy;
    case SportMannerEnum::Chop: return new ChopSportStrategy;


    default: return nullptr;
  }
}

class Sport // Родительский (базовый) класс
{
private: // "Закрытые" компоненты (не доступны в унаследованных классах)
    SportDanger Danger;
    double Hard;

    SportStrategy *SportManner;

    void DoSportUsingStrategy()
    {
      if(SportManner == nullptr)
      {
        cout << "Nichego!";
        return;
      }
      else
      {
        SportManner->Competition();
      }
    }

    void DetectOlimpicYesOrNo()
    {
      if(IsYes())
      {
          cout << "Yes";
      }
      else
      {
          cout << "No";
      }
    }


protected: // "Защищенные" компоненты (доступны в унаследованных классах, но не доступны внешнему наблюдателю)
    bool OlympicIsYes;

public: // "Открытые" компоненты, определяющие интерфейс класса

    Sport(SportDanger danger) : Danger(danger), Hard(0.0), OlympicIsYes(false), SportManner(nullptr)
    {
        // Значение инициализируется случайным числом 0 или 1
        OlympicIsYes = static_cast<bool>(rand()%2);
    }

    virtual ~Sport() // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)
    {
      if(SportManner != nullptr) delete SportManner;
    }
    bool IsYes() const { return OlympicIsYes; }
    SportDanger GetDanger() const { return Danger; }
    double GetHard() const { return Hard; }
    virtual void PrintType() = 0;
    virtual void Separate() = 0;
    // Функция с реализацией
    void Competition()
    {
        // 1. Вывести название
        PrintType();
        cout << " : ";

        // 2. Определить, олимпийский вид спорта или нет
        DetectOlimpicYesOrNo();
        cout << " : ";

        // 2.1 разделить
        Separate();
        cout << " : ";

        DoSportUsingStrategy();

        cout << endl;
    }

    void SetSportManner(SportStrategy *sportManner) { SportManner = sportManner; }
};

class Cheer : public Sport // Класс-наследник
{
public:
    Cheer();
    ~Cheer() {}

    void PrintType() { cout << "Cheer"; }
    void Separate() { cout << "Separate them using special rules"; }
};

// Реализация конструктора
Cheer::Cheer() : Sport(SportDanger::Safely)
{
  // Определить стратегию по умолчанию
  SetSportManner(CreateSportStrategy(SportMannerEnum::BiteOff));
}

class Swimming : public Sport // Класс-наследник
{
public:
    Swimming() : Sport(SportDanger::Safely) { SetSportManner(CreateSportStrategy(SportMannerEnum::Swallow)); }
    ~Swimming() {}

    void PrintType() { cout << "Swimming"; }
    void Separate() { cout << "Separate using small knife"; }
};

class Running : public Sport // Класс-наследник
{
public:
    Running() : Sport(SportDanger::Medium) { SetSportManner(CreateSportStrategy(SportMannerEnum::Chop)); }
    ~Running() {}

    void PrintType() { cout << "Running"; }
    void Separate() { cout << "Separate using chief knife"; }
};

// Реализация паттерна "Фабричный метод"

enum class SportType : int
{
    Cheer = 1,
    Running = 2,
    Swimming = 3,

    Undefined = 0 // На всякий случай
};

Sport *CreateSport(SportType type)
{
    Sport *newSport = nullptr;

    if(type == SportType::Cheer)
    {
        newSport = new Cheer;
    }
    else if(type == SportType::Running)
    {
        newSport = new Running;
    }
    else if(type == SportType::Swimming)
    {
        newSport = new Swimming;
    }

    return newSport;
}

// Декоратор итератора

class SportDangerDecorator : public IteratorDecorator<class Sport*>
{
private:
    SportDanger TargetDanger;

public:
    SportDangerDecorator(Iterator<Sport*> *it, SportDanger danger)
    : IteratorDecorator<Sport*>(it), TargetDanger(danger) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetDanger() != TargetDanger)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetDanger() != TargetDanger);
    }
};

// Декоратор итератора

class SportOlympicDecorator : public IteratorDecorator<class Sport*>
{
private:
    bool TargetYes;

public:
    SportOlympicDecorator(Iterator<Sport*> *it, bool isGood)
    : IteratorDecorator<Sport*>(it), TargetYes(isGood) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsYes() != TargetYes)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsYes() != TargetYes);
    }
};

void SportEmAll(Iterator<Sport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Sport *currentSport = it->GetCurrent();
        currentSport->Competition();
    }
}

void EatEmAllGood(Iterator<Sport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Sport *currentSport = it->GetCurrent();
        if(!currentSport->IsYes()) continue;

        currentSport->Competition();
    }
}


void EatEmAllCheer(Iterator<Sport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Sport *currentSport = it->GetCurrent();
        if(currentSport->GetDanger() != SportDanger::Safely) continue;

        currentSport->Competition();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    size_t N = 30;

    // Массив

    ArrayClass<Sport*> sportArray;
    for(size_t i=0; i<N; i++)
    {
        int sport_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        SportType sport_type = static_cast<SportType>(sport_num);
        Sport *newSport = CreateSport(sport_type);
        sportArray.Add(newSport);
    }

    wcout << L"Размер массива спорта: " << sportArray.Size() << endl;

    // Связанный список

    list<Sport*> sportVector;
    for(size_t i=0; i<N; i++)
    {
        int sport_num = rand()%3+1; // Число от 1 до 3
        SportType sport_type = static_cast<SportType>(sport_num);
        Sport *newSport = CreateSport(sport_type);
        sportVector.push_back(newSport); // Добавить новый спорт в конец
    }

    wcout << L"Размер списка спорта: " << sportVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Competition all in a simple loop:" << endl;
    for(size_t i=0; i<sportArray.Size(); i++)
    {
        Sport *currentSport = sportArray[i];
        currentSport->Competition();
    }

    // Обход всех элементов при помощи итератора
    cout << endl << "Competition all using iterator:" << endl;
    Iterator<Sport*> *allIt = sportArray.GetIterator();
    SportEmAll(allIt);
    delete allIt;

    cout << endl << "Competition all good using iterator:" << endl;
    Iterator<Sport*> *goodIt = new SportOlympicDecorator(sportArray.GetIterator(), true);
    SportEmAll(goodIt);
    delete goodIt;

    cout << endl << "Competition all swimming using iterator:" << endl;
    Iterator<Sport*> *swimmingIt = new SportDangerDecorator(sportArray.GetIterator(), SportDanger::Safely);
    SportEmAll(swimmingIt);
    delete swimmingIt;

    cout << endl << "Competition all good swimming using iterator:" << endl;
    Iterator<Sport*> *goodSwimmingIt =
        new SportOlympicDecorator(new SportDangerDecorator(sportArray.GetIterator(), SportDanger::Safely), true);
    SportEmAll(goodSwimmingIt);
    delete goodSwimmingIt;

    // Демонстрация работы адаптера
    cout << endl << "Competition all good swimming using adapted iterator (another container):" << endl;
    Iterator<Sport*> *adaptedIt = new ConstIteratorAdapter<std::list<Sport*>, Sport*>(&sportVector);
    Iterator<Sport*> *adaptedSwimmingIt = new SportOlympicDecorator(new SportDangerDecorator(adaptedIt, SportDanger::Safely), true);
    SportEmAll(adaptedSwimmingIt);
    delete adaptedSwimmingIt;
    // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedSwimmingIt

    return 0;
}
