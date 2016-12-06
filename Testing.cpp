#include <iostream>
#include <string>
#include <string.h>


#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>

using namespace std;

class Owning_Str{

public :
    Owning_Str(char* p = 0) throw()     : shared(p!=0), _strbuf(p) {}


    ~Owning_Str();        
   
    Owning_Str(char*,int);   
    int length() const;
    char char_place(int) const;                               
    void marked(int);                             
    void Appnd(char);                          
    void Print();



	Owning_Str(const Owning_Str& x) throw(): shared(x.shared), _strbuf(x.Release_Mem()) 
	{
	}



    Owning_Str& operator =(const Owning_Str &x )
    { 

    	if (&x != this) {
            if (_strbuf != x._strbuf)
            {
                if (shared)
                {
                	delete[] _strbuf;
                	delete _strbuf;
                }
                
                shared = x.shared;
            }
            else if (x.shared) shared = true;
            delete[] _strbuf;
            _strbuf = x.Release_Mem();

            _length = x._length;
        }

        return *this;
    }



    char* Release_Mem()  const throw()
    {
    	(const_cast<Owning_Str* >(this))->shared = false; 
    	return _strbuf;
    }


private:
    bool shared;
    int _length;
    char* _strbuf;                                   
                                           

};


class Ref_Linking_Str{

public :
    
    Ref_Linking_Str(Ref_Linking_Str* p = 0) throw()   : _strbuf(0) 
    {
        Previous = NexPtr = this;
    }


    ~Ref_Linking_Str();    
    int length() const;           
    char char_place(int) const;                              
    void marked(int);                           
    void Appnd(char);                          
    void Print();


    char* get() const throw()   {return _strbuf;}
    

	Ref_Linking_Str(const Ref_Linking_Str& x)
	{
        Fetch(x);
        _length = x._length;
        
	}



    Ref_Linking_Str& operator= (const Ref_Linking_Str &x )
    { 

    	if (this != &x) {
            rel();
            Fetch(x);
        }
        return *this;
    }

    bool single()   const throw()  
     {
     	if(Previous == this && NexPtr == this)
     		return 1;
     	else
     		return 0;
     }



    


private:
    int _length;                                  
    char*    _strbuf; 
    
    
    Ref_Linking_Str*   Previous;
    Ref_Linking_Str*   NexPtr;
    
    void Fetch(const Ref_Linking_Str& x) throw()
    {
        _strbuf = x._strbuf;
        _length = x._length;
        NexPtr = x.NexPtr;
        NexPtr->Previous = this;
        (const_cast<Ref_Linking_Str*>(&x))->NexPtr = this;
    }

    void rel()
    { 
        if (single()) 
        	delete[] _strbuf;
        else
        {
            Previous->NexPtr = NexPtr;
            NexPtr->Previous = Previous;
            Previous = NexPtr = 0;
            
        }
       
    }

};



class Ref_Counting_Str{

public :
    Ref_Counting_Str() throw()   : temp_C(0) 
    {
        _length = 0;   
    }


    ~Ref_Counting_Str();         
    int length() const;       
    char char_place(int) const;                            
    void marked(int);                             
    void Appnd(char);                          
    void Print();


    char* get()        const throw()   {return _strbuf;}
    

    Ref_Counting_Str(const Ref_Counting_Str& x)
    {
        fetch(x.temp_C);
        _length = x._length; 
    }



    Ref_Counting_Str& operator= (const Ref_Counting_Str &x )
    { 

        if (this != &x) 
        {
            rel();
            fetch(x.temp_C);
        }
        _strbuf = x._strbuf;
        _length = x._length;
        return *this;
    }

    bool single()   const throw()
        {return (temp_C ? temp_C->count == 1 : true);}



    


private:
    struct counter 
    {
        counter(unsigned c = 1) :  count(c) {}
        unsigned    count;
    }* temp_C;  
                                       
    char*    _strbuf; 
    int _length;

    void fetch(counter* c) throw()
    { 
        
        temp_C = c;
        if (c) ++c->count;
    }

    void rel()
    { 
        
        if (temp_C) {
          if (--temp_C->count == 0) 
            {
                delete[] _strbuf;
                delete temp_C;
            }
        }
    }

};


class Coping_Str{

public :
     Coping_Str(char* p = 0) throw()     : _strbuf(p) 
     {
        _length = 0;
     }
    
    ~Coping_Str();               
    Coping_Str(const Coping_Str&);              
    Coping_Str(char*,int);   
    int length() const;     
    char char_place(int) const;                             
    void marked(int);                             
    void Appnd(char);                          
    void Print();

    Coping_Str& operator= (const Coping_Str &x )
    { 
        if (this != &x)
        {
            delete[] _strbuf;
            _strbuf = new char [x._length];
            strcpy(_strbuf, x._strbuf);
            _length = x._length;       
        }
        return *this;
    }

private:
	int _length;
	char* _strbuf;                                   
                                           

    


};


class Testing
{
public:
	Testing(){};

	void Coping_Pointer();
	void Owning_Pointer();
	void Reference_Counting_Ptr();
	void Reference_Linking_Ptr();


	void Coping_Length();
	void Owning_Length();
	void Ref_Counting_Lnth();
	void Ref_Linking_Lnth();


	void Coping_Appnd();
	void Owning_Appnd();
	void Ref_Counting_Apnd();
	void Ref_Linking_Apnd();	



	void Coping_Print();
	void Owning_Print();
	void Ref_Counting_Prnt();
	void Ref_Linking_Prnt();	

};




void Testing::Coping_Pointer()
{
	Coping_Str obj1;
	obj1.marked(10);
	obj1.Appnd('A');

	Coping_Str obj2(obj1);
	int i;
	for(i = 0; i < 2;++i)
	{
		if(obj1.char_place(i) != obj2.char_place(i))
		{
			cout<<"-->\tTest Failed"<<endl;
			return;
		}
	}
	cout <<"-->\tTest Passed"<<endl;
}



void Testing::Owning_Pointer()
{
	Owning_Str* obj1 = new Owning_Str();
	obj1->marked(10);
	obj1->Appnd('A');
	Owning_Str* obj2 = new Owning_Str(*obj1);
	obj2->marked(10);
	*obj2 = *obj1;
	int i;
	for(i = 0; i < 2;++i)
	{
		if(obj1->char_place(i) != obj2->char_place(i))
		{
			cout<<"-->\tTest Failed"<<endl;
			return;
		}
	}
	cout <<"-->\tTest Passed"<<endl;
	
}







Coping_Str::Coping_Str(const Coping_Str& x)
{
    _strbuf = new char [x._length];
    strcpy(_strbuf, x._strbuf);
    _length = x._length;       
}


Coping_Str::Coping_Str(char* str,int len)
{
    _strbuf = str;
    _length = len;
}



Coping_Str::~Coping_Str()  
{

    delete[] _strbuf;
   
}



int Coping_Str::length() const{
	return _length;
}


char Coping_Str::char_place(int x) const
{
	return _strbuf[x];
}


void Coping_Str::marked(int x)
{
	_strbuf = new char[x];
}

void Coping_Str::Appnd(char alphabet)
{
	_strbuf[_length] = alphabet;
	_length += 1;
}

void Coping_Str::Print()
{
    
    for (int i = 0; i < _length ; ++i)
    {
         cout<<*(_strbuf + i) ;
    }
    
}

void Testing::Reference_Counting_Ptr()
{
	Ref_Counting_Str obj1;
	obj1.marked(10);
	obj1.Appnd('A');
	
	Ref_Counting_Str obj2;

	obj2.marked(10);

	obj2 = obj1;
	int i;
	for( i = 0; i < 2;++i)
	{
		if(obj1.char_place(i) != obj2.char_place(i))
		{
			cout<<"-->\tTest Failed"<<endl;
			return;
		}
	}
	cout <<"-->\tTest Passed"<<endl;

}


void Testing::Reference_Linking_Ptr()
{
	Ref_Linking_Str* obj1 = new Ref_Linking_Str();
	obj1->marked(10);
	obj1->Appnd('A');

	Ref_Linking_Str* obj2 = new Ref_Linking_Str();
	obj2->marked(10);
	*obj1 = *obj2;
        int i;
	for( i = 0; i < 2;++i)
	{
		if(obj1->char_place(i) != obj2->char_place(i))
		{
			cout<<"-->\tTest Failed"<<endl;
			return;
		}
	}
	cout <<"-->\tTest Passed"<<endl;
	

}



void Testing::Coping_Length()
{
	Coping_Str t_obj;
	t_obj.marked(10);
	t_obj.Appnd('A');

	int len = t_obj.length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
}



void Testing::Owning_Length()
{
	Owning_Str* obj1 = new Owning_Str();
	obj1->marked(10);
	obj1->Appnd('A');

	int len = obj1->length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
	
}


void Testing::Ref_Counting_Lnth()
{
	Ref_Counting_Str obj1;
	obj1.marked(10);
	obj1.Appnd('A');
	obj1.Appnd('B');

	int len = obj1.length();
	if(len == 2)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;

}


void Testing::Ref_Linking_Lnth()
{
	Ref_Linking_Str* obj1 = new Ref_Linking_Str();
	obj1->marked(10);
	obj1->Appnd('A');

	int len = obj1->length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
	
}



void Testing::Coping_Appnd()
{
	Coping_Str t_obj;
	t_obj.marked(10);
	t_obj.Appnd('A');

	int len = t_obj.length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
}



void Testing::Owning_Appnd()
{
	Owning_Str* obj1 = new Owning_Str();
	obj1->marked(10);
	obj1->Appnd('A');

	int len = obj1->length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
	
}


void Testing::Ref_Counting_Apnd()
{
	Ref_Counting_Str obj1;
	obj1.marked(10);
	obj1.Appnd('A');
	obj1.Appnd('B');

	int len = obj1.length();
	if(len == 2)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;

}


void Testing::Ref_Linking_Apnd()
{
	Ref_Linking_Str* obj1 = new Ref_Linking_Str();
	obj1->marked(1);
	obj1->Appnd('A');

	int len = obj1->length();
	if(len == 1)
		cout <<"-->\tTest Passed"<<endl;
	else
		cout <<"-->\tTest Failed"<<endl;
	
}


void Testing::Coping_Print()
{
	Coping_Str t_obj;
	t_obj.marked(1);
	t_obj.Appnd('A');
	t_obj.Print();

	cout<<endl;
	if(t_obj.char_place(0) == 'A')
	{
		cout <<"-->\tTest Passed"<<endl;	
	}
	else
	{
		cout <<"-->\tTest Failed"<<endl;
	}
}



void Testing::Owning_Print()
{
	Owning_Str* obj1 = new Owning_Str();
	obj1->marked(1);
	obj1->Appnd('A');
	obj1->Print();
	
	cout<<endl;

	if(obj1->char_place(0) == 'A')
	{
		cout <<"-->\tTest Passed"<<endl;	
	}
	else
	{
		cout <<"-->\tTest Failed"<<endl;
	}
	
}


void Testing::Ref_Counting_Prnt()
{
	Ref_Counting_Str obj1;
	obj1.marked(1);
	obj1.Appnd('A');
	obj1.Print();

	cout<<endl;
	if(obj1.char_place(0) == 'A')
	{
		cout <<"-->\tTest Passed"<<endl;	
	}
	else
	{
		cout <<"-->\tTest Failed"<<endl;
	}

}


void Testing::Ref_Linking_Prnt()
{
	Ref_Linking_Str* obj1 = new Ref_Linking_Str();
	obj1->marked(1);
	obj1->Appnd('A');
	
	obj1->Print();
	cout<<endl;

	if(obj1->char_place(0) == 'A')
	{
		cout <<"-->\tTest Passed"<<endl;	
	}
	else
	{
		cout <<"-->\tTest Failed"<<endl;
	}

	
}

Ref_Counting_Str::~Ref_Counting_Str()  
{
    rel();
    
}



int Ref_Counting_Str::length() const{
    return _length;
}


char Ref_Counting_Str::char_place(int x) const
{
    return _strbuf[x];
}


void Ref_Counting_Str::marked(int x)
{
    _strbuf = new char[x];
    temp_C = new counter();
}

void Ref_Counting_Str::Appnd(char alphabet)
{
    if(temp_C->count > 1)
    {
        --temp_C->count;
        temp_C = new counter();
        char* temp = new char[_length];
        strncpy(temp,_strbuf,_length);
        _strbuf = new char[_length];
        strncpy(_strbuf,temp,_length);
        _strbuf[_length] = alphabet;
        _length += 1;
        cout<<"New Copy Created"<<endl;
        delete[] temp;
       

    }
    else
    {
        _strbuf[_length] = alphabet;    
        _length += 1;
    }
}

void Ref_Counting_Str::Print()
{
    int i;
    for (i = 0; i < _length; ++i)
    {
         cout<<*(_strbuf + i) ;
    }
}
Ref_Linking_Str::~Ref_Linking_Str()  
{
	
	rel();
	
}



int Ref_Linking_Str::length() const{
	return _length;
}


char Ref_Linking_Str::char_place(int x) const
{
	return _strbuf[x];
}


void Ref_Linking_Str::marked(int x)
{
	_strbuf = new char[x];
}

void Ref_Linking_Str::Appnd(char alphabet)
{
	if(single())
	{
		
		_strbuf[_length] = alphabet;
		_length += 1;
	}
	else
	{
		char* temp = new char[_length];
		strncpy(temp,_strbuf,_length);
		rel();
		_strbuf = new char[_length];
		strncpy(_strbuf,temp,_length);
		_strbuf[_length] = alphabet;
		_length += 1;
		delete[] temp;
		NexPtr = Previous = this;

	}
	
}

void Ref_Linking_Str::Print()
{
    
    for (int i = 0; i < _length ; ++i)
    {
         cout<<*(_strbuf + i) ;
    }
}

Owning_Str::Owning_Str(char* str,int len)
{
    _strbuf = str;
    _length = len;
}



Owning_Str::~Owning_Str()  
{
	if (shared)
	{
		delete[] _strbuf;
	}
	delete _strbuf;
	
}



int Owning_Str::length() const{
	return _length;
}


char Owning_Str::char_place(int x) const
{
	return _strbuf[x];
}


void Owning_Str::marked(int x)
{
	_strbuf = new char[x];
}

void Owning_Str::Appnd(char alphabet)
{
	_strbuf[_length] = alphabet;
	_length += 1;
}

void Owning_Str::Print()
{
    int i;
    for (i = 0; i < _length+1 ; ++i)
    {
         cout<<*(_strbuf + i) ;
    }
    
}


int main(int argc, char const *argv[])
{

	
	Testing obj;
        cout<<"\t \tPOINTERS TESTS:"<<endl;
	obj.Coping_Pointer();
	obj.Owning_Pointer();
	obj.Reference_Counting_Ptr();
	obj.Reference_Linking_Ptr();	


        cout<<"\t \tLENGTH TESTS:"<<endl;
	obj.Coping_Length();
	obj.Owning_Length();
	obj.Ref_Counting_Lnth();
	obj.Ref_Linking_Lnth();

	cout<<"\t \tAPPENDING TESTS:"<<endl;
	obj.Coping_Appnd();
	obj.Owning_Appnd();
	obj.Ref_Counting_Apnd();
	obj.Ref_Linking_Apnd();

	cout<<"\t \tPRINTING TESTS:"<<endl;
	obj.Coping_Print();
	obj.Owning_Print();
	obj.Ref_Counting_Prnt();
	obj.Ref_Linking_Prnt();

	return 0;
}
