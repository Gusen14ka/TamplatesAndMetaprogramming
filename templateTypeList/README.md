In this branch, I tried to implement a template class TypeList<Ts...>, which is a container for types of an unknown number of parameters. I have implemented functions for this class:

1) Length<TypeList> (the length of the list)

2) PushFront<Type List, New> (add type to the beginning)

3) indexOf<TypeList, T> (type position in the list or -1)

4) Erase<Type List, T> (delete all occurrences of the type)