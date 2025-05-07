#ifndef PERSON_H
#define PERSON_H

// #include <cstdio>
#include <string>
#include <ctime>

typedef struct  {
    int series;
    int number;
} PersonID;

class Person {
    private:
        PersonID ID;
        std::string firstName;
        std::string lastName;
        std::string middleName;
        time_t birthDate;

// char* copyString(const char* str) {
//         if (!str) return nullptr;
//         char* copy = new char[strlen(str) + 1];
//         strcpy(copy, str);
//         return copy;
// }

    public:
    Person() : ID({0, 0}), firstName(""), lastName(""), middleName(""), birthDate(0) {}

    Person(PersonID id, const std::string& firstName, const std::string& lastName, const std::string& middleName, time_t birthDate):
    ID(id), firstName(firstName), lastName(lastName),
    middleName(middleName), birthDate(birthDate) {}



    PersonID GetID() const {
        return ID;
    }
    const std::string& GetFirstName() const {
        return firstName;
    }
    const std::string& GetMiddleName() const {
        return middleName;
    }
    const std::string& GetLastName() const {
        return lastName;
    }
    time_t GetBirthDate() const {
        return birthDate;
    }
    // ~Person() {
    //     delete[] firstName;
    //     delete[] lastName;
    //     delete[] middleName;
    // }
    bool operator<(const Person &other) const {
        return this->birthDate < other.birthDate;
    }
};

#endif // PERSON_H