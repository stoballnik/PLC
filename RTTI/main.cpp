#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

std::unordered_map<std::string, std::unordered_set<std::string>> classHierarchy;

size_t GetHash(std::string className) {
	std::hash<std::string> hashFunc;
	return hashFunc(className);
};

struct typeInfo {
	typeInfo(std::string name) : name_(name), hash_(GetHash(name)) {};

	std::string name_;
	size_t hash_;
};

std::unordered_set<std::string> GetParents(std::string str) {
	std::unordered_set<std::string> result;
	std::string name("");
	for (auto &i: str) {
		if (i == ' ') {
			name = "";
		}
		else if (i == ',') {
			result.insert(name);
			name = "";
		}
		else {
			name += i;
		}
	}
	result.insert(name);
	return result;
};

bool IsParent(std::string child, std::string parent) {
	if (child == parent) {
		return true;
	}
	for (auto &i : classHierarchy[child]) {
		if (IsParent(i, parent)) {
			return true;
		}
	}
	return false;
};

#define TYPEID(obj_ptr) obj_ptr->GetInfo()

#define TYPEINFO( className ) className::GetInfo()

#define BASE_CLASS(class_name) class class_name { \
  public: \
    class_name(): name_(#class_name) {\
		classHierarchy.emplace(#class_name, class_name::parents_);\
	}; \
	typeInfo GetInfo() { \
		return class_name::typeInfo_; \
	} \
	std::string GetName(){\
		return name_;\
	}\
  private: \
	std::string name_; \
    static std::unordered_set<std::string> parents_; \
    static typeInfo typeInfo_; \



#define DERIVED_CLASS(class_name, ...) class class_name: __VA_ARGS__ { \
  public: \
    class_name(): name_(#class_name) {\
		class_name::parents_ = GetParents(#__VA_ARGS__); \
        classHierarchy.emplace(#class_name, class_name::parents_); \
	}; \
    typeInfo GetInfo() { \
        return class_name::typeInfo_; \
    } \
	std::string GetName(){\
		return name_;\
	}\
  private: \
	std::string name_; \
    static std::unordered_set<std::string> parents_; \
    static typeInfo typeInfo_; \

#define END_CLASS(class_name) };\
std::unordered_set<std::string> class_name::parents_; \
typeInfo class_name::typeInfo_ = typeInfo(#class_name); \

#define DYNAMIC_CAST(to, objectPtr) (IsParent(objectPtr->GetName(), #to)) ? reinterpret_cast<to*>(objectPtr) : nullptr

BASE_CLASS(A)
public:
	void function() {
		std::cout << "This is a function of class A" << std::endl;
	};
END_CLASS(A)

BASE_CLASS(B)
public:
	void function() {
		std::cout << "This is a function of class B" << std::endl;
	};
END_CLASS(B)

BASE_CLASS(C)
public:
	void function() {
		std::cout << "This is a function of class C" << std::endl;
	};
END_CLASS(C)

BASE_CLASS(D)
public:
	void function() {
		std::cout << "This is a function of class D" << std::endl;
	};
END_CLASS(D)

BASE_CLASS(E)
public:
	void function() {
		std::cout << "This is a function of class E" << std::endl;
	};
END_CLASS(E)

BASE_CLASS(F)
public:
	void function() {
		std::cout << "This is a function of class F" << std::endl;
	};
END_CLASS(F)

DERIVED_CLASS(G, public F, public D)
public:
	void function() {
		std::cout << "This is a function of class G" << std::endl;
	};
END_CLASS(G)

DERIVED_CLASS(H, public G, public C)
public:
	void function() {
		std::cout << "This is a function of class H" << std::endl;
	};
END_CLASS(H)

DERIVED_CLASS(J, public A, public B, public C)
public:
	void function() {
		std::cout << "This is a function of class J" << std::endl;
	};
END_CLASS(J)

void function() {
	std::cout << "Bad cast" << std::endl;
}
int main() {
	B b;

	A* a = DYNAMIC_CAST(A, (&b));
	std::cout << "B to A" << std::endl;
	if (a) {
		a->function();
	}
	else {
		function();
	}
	J j;
	H h;
	H* h2 = DYNAMIC_CAST(H, (&j));
	std::cout << "J to H" << std::endl;
	if (h2) {
		h2->function();
	}
	else {
		function();
	}

	D* d = DYNAMIC_CAST(D, (&h));
	std::cout << "H to D" << std::endl;
	if (d) {
		d->function();
	}
	else {
		function();
	}

	C* c = DYNAMIC_CAST(C, (&h));
	std::cout << "H to C" << std::endl;
	if (c) {
		c->function();
	}
	else {
		function();
	}

	c = DYNAMIC_CAST(C, (&j));
	std::cout << "J to C" << std::endl;
	if (c) {
		c->function();
	}
	else {
		function();
	}

	typeInfo info = TYPEID((&j));
	std::cout << "Info of class J:\nName: " << info.name_ << " ; Hash: " << info.hash_ << std::endl;
	info = TYPEID((&h));
	std::cout << "Info of class H:\nName: " << info.name_ << " ; Hash: " << info.hash_ << std::endl;
	return 0;
}