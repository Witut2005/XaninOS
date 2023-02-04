int Constant(int x)
    {return x;}

int Linear(int x,int a,int b)
    {
        int RESULT;
        RESULT=a*x;
        RESULT+=b;
        return RESULT;
    }

int Quadratic(int x,int a,int p,int q)
    {
        int RESULT;
        RESULT=x-p;
        RESULT=RESULT*RESULT;
        RESULT=RESULT*a;
        RESULT=RESULT+q;
        return RESULT;
    }
    
int Rational(int x,int a,int p,int q)
    {
        int RESULT;
        RESULT=x-p;

        if(RESULT!=0)
        RESULT=a/RESULT;
        else 
        RESULT=0;
        
        RESULT=RESULT+q;
        return RESULT;
    }