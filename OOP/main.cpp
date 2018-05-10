#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

#define VIRTUAL_CLASS(class_name) struct class_name {\
    std::unordered_map<std::string, std::function<void(void*)>> functions;

#define BASE_CONSTRUCTOR(class_name, CODE) class_name() { CODE };

#define ADD_FUNCTION(class_name, function_name)\
    this->functions.emplace(#function_name, std::function<void(void*)>(this->function_name));

#define START_DECLARE_METHOD(class_name, function_name)\
    static void function_name(void* pointer) {\
        class_name* this_ = (class_name*)pointer;\
        std::cout << #class_name << "::" << #function_name << " ";

#define END_DECLARE_METHOD std::cout << std::endl; };

#define END(class_name) };

#define VIRTUAL_CALL(pointer, function_name)\
(pointer->functions[#function_name])(pointer);                                                

#define VIRTUAL_CLASS_DERIVED(base_class_name, derived_class_name) struct derived_class_name {\
    std::unordered_map<std::string, std::function<void(derived_class_name*)>> functions;

#define DERIVED_CONSTRUCTOR(base_class_name, derived_class_name, CODE) derived_class_name() {\
    CODE\
    base_class_name base_class;\
    for (auto i = base_class.functions.begin(); i != base_class.functions.end(); ++i) {\
        if (this->functions.count(i->first) == 0) {\
            this->functions.emplace(i->first, std::function<void(derived_class_name*)>(i->second));\
        }\
    }\
};                                                                                                      


// базовый класс
VIRTUAL_CLASS(Base)

int a;

// методы
START_DECLARE_METHOD(Base, Both)
std::cout << this_->a;
END_DECLARE_METHOD

START_DECLARE_METHOD(Base, OnlyBase)
END_DECLARE_METHOD

BASE_CONSTRUCTOR(Base,
	ADD_FUNCTION(Base, Both)
	ADD_FUNCTION(Base, OnlyBase)
	)
END(Base)

	// класс-наследник
VIRTUAL_CLASS_DERIVED(Base, Derived)

int b = 1;

// методы
START_DECLARE_METHOD(Derived, Both)
std::cout << this_->b;
END_DECLARE_METHOD

START_DECLARE_METHOD(Derived, OnlyDerived)
END_DECLARE_METHOD

DERIVED_CONSTRUCTOR(Base, Derived,
	ADD_FUNCTION(Derived, Both)
	ADD_FUNCTION(Derived, OnlyDerived)
	)
END(Derived)

int main() {
	Base base;
	base.a = 0;
	Derived derived;
	Base* reallyDerived = reinterpret_cast<Base*>(&derived);

	VIRTUAL_CALL((&base), Both);
	VIRTUAL_CALL(reallyDerived, Both);
	VIRTUAL_CALL(reallyDerived, OnlyBase);
	VIRTUAL_CALL(reallyDerived, OnlyDerived);
	return 0;
}
