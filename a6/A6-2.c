#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*/Users/midoritakeuchi/Desktop/A6_2.c
* Person
*/

struct Person_class {
	void*	super;
	void (* toString) (void* this, char* buf, int bufSize);

};

struct Person {
	struct Person_class*  class;
	char* name;
};

void Person_toString(void* this, char* buf, int bufSize) {
    struct Person* obj = this;
    snprintf(buf, bufSize, "Name: %s", obj->name);
}

struct Person_class Person_class_obj = {NULL, Person_toString};

struct Person* new_Person(char* name) {
    struct Person* obj = malloc(sizeof (struct Person));
    obj->class = &Person_class_obj;
    obj->name = strdup(name);
    return obj;
}

/*
 * Class Student extends Person
 */

struct Student_class {
    struct Person_class* super;
    void (* toString) (void* this, char* buf, int bufSize);
};

struct Student {
    struct Student_class* class;
    char* name;
    int sid;
   
};

void Student_toString(void* this, char* buf, int bufSize) {
    struct Student* obj = this;
    snprintf(buf, bufSize, "Name: %s, SID: %d", obj->name, obj->sid);
}

struct Student_class Student_class_obj = {&Person_class_obj, Student_toString};

struct Student* new_Student(char* name, int sid) {
    struct Student* obj = malloc(sizeof (struct Student));
    obj->class = &Student_class_obj;
    obj->name = strdup(name);
    obj->sid = sid;
    return obj;
}

/*
 * Main
 */

int main (int argc, char** argv) {
    struct Person* alex = new_Person("Alex");
    struct Student* alice = new_Student("Alice", 300);
    
    struct Person* people[2] = {alex, (struct Person*) alice};
    
    char buf[100];
    for (int i=0; i < 2; i++){
        people[i]->class->toString(people[i], buf, sizeof(buf));
        printf("%s\n", buf);
    }
}




    
