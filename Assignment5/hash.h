//Name- Gurkirat Singh Arora
//Date- 5th August, 2022
#include <iostream>
#include <cstring>
#include<cmath>

using namespace std;


const unsigned scale = 225;


// * * * replace with your own personal modulus * * * //
const unsigned M = 3931;



// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
    private:
        T * A[M];



        // * * * Add attributes and helper methods here if you wish. * * * //


       // unsigned size; // * * * remove this * * * //



    public:
        // Desc:  Default constructor.  Initialize table to NULL.
        Set() {
            memset(A, 0, sizeof(T *)*M);
        }


        // Desc:  Insert x into the Set, according to its ->key.
        //        If ->key is already present, then replace it by x.
        //        Collisions are resolved via quadratic probing sequence.
        // Post:  returns the table index where x was inserted
        //        returns -1 if x was not inserted
        int insert(T * x);


        // Desc:  Returns T * x such that x->key == key, if and only if
        //        key is in the Set.
        //        Returns NULL if and only if key is not in the Set.
        // Post:  Set is unchanged
        T * search(string key) const;
    int hashvalue(string key)const;
};


// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T * x)
{
    int hvalue= hashvalue(x->key);
    int i= hvalue;
    int count=1;
    int j=0;
    while(j<(M/2+1))
    {
        if(A[i]==NULL)
        {
            A[i]=x;
            return i;
        }
        else if(A[i]->key==x->key)
        {
            T * temp=A[i];
            A[i]=x;
            return i;
        }
        else
        {
            i= static_cast<int>(hvalue + pow(count,2)) % M;
        }
        count++;
        j++;
    }
    return -1;
}
template <class T>
T * Set<T>::search(string key) const
{
    int hvalue= hashvalue(key);
    int i= hvalue;
    int count=1;
    int j=0;
    while(j<M/2+1)
    {
        if(A[i]==nullptr)
        {
            return NULL;
        }
        else if(A[i]->key==key)
        {
            return A[i];
        }
        else
        {
            i= static_cast<int>(hvalue +pow(count,2))% M;
        }
        count++;
        j++;
    }
    return NULL;
}
template <class T>
int Set<T>::hashvalue(string key)const
{
    int value;
    if(key[0]>='a' && key[0]<='z')
        value= key[0]-87;
    else if(key[0]>='A' && key[0]<='Z')
        value= key[0]-29;
    else
        value= 62;
    for(int i=1;i<key.length();i++)
    {
        if(key[i]>='0' && key[i]<='9')
            value=(value*64 + (key[i]-48)) % M;
        else if(key[i]>='a' && key[i]<='z')
            value=(value*64 + (key[i]-87)) % M;
        else if(key[i]>='A' && key[i]<='Z')
            value=(value*64 + (key[i]-29)) % M;
        else
            value=(value*64 + 62) % M;
    }
    value =scale*value%M;
    return value;
}


