#include <xaninApps/grapher/functions.h>
#include <libc/stdlibx.h>
#include <libc/stdiox.h>
//xanin 80x28
// char 219 is white square
unsigned short DispX=79;
unsigned short DispY=27;

//traz was here

int grapher(){
    bool RunFlag= true;

    /*
    remains from the cmd version

    cout<<"Resolution"<<endl;

    ResInputX:
    cout<<"x: ";    cin>>DispX;
    if(DispX<=1){cout<<"Must be bigger than 1!"<<endl;goto ResInputX;}

    ResInputY:
    cout<<"y: ";    cin>>DispY;
    if(DispY<=1){cout<<"Must be bigger than 1!"<<endl;goto ResInputY;}
    
    //DispX=79;
    //DispY=27;
    cout<<"Drawing @: "<<DispX<<"x"<<DispY<<" res."<<endl;
    */

    unsigned char D[79][27]= {};
    unsigned short POSX=0;
    unsigned short POSY=0;

    short CenterX=0;
    short CenterY=0;
    short ValX=0;
    short ValY=0;
    int ValueA=0;
    int ValueP=0;
    int ValueQ=0;
    unsigned short FunctionType=0;
    
    unsigned short select=0;
    unsigned short RunAgain=0;
    unsigned short Clearing=0;

    xprintf("?Draw axies?\n[Y/any]: ");
    //cout<<"?Draw axies?"<<endl<<"[Y/any]: ";
    xscanf("%d", &select);
    //cin>> select;


    while(RunFlag==true)
    {
    RunFlag=false;
    screen_clear();

    if(select==121||select==89||select==49)
    //121 -> y 89 -> Y
    {
        //////////////////////////////////
        //axies drawing (in array!!!! not on screen!)

            //Draw X axis
        POSY=DispY/2;
        while (POSX<=DispX-2)
        {
            D[POSX][POSY]=196;//horizontal line
            POSX++;
        }
        D[POSX][POSY]=88;//Add X letter

        POSX=DispX/2;       //
        POSX++;             //add unit mark on X axis
        D[POSX][POSY]=250;  //

        POSY=0;
        POSX=0;

        //Draw Y axis
        POSX=DispX/2;

        D[POSX][POSY]=89;   //add Y letter
        POSY++;

        while (POSY<=DispY-1)
        {
            D[POSX][POSY]=179;// vertical line
            POSY++;
        }

        POSY=DispY/2;       //
        POSY--;             //add unit mark on Y axis
        D[POSX][POSY]=250;  //

        POSY=0;
        POSX=0;
    }
      //axies drawing
     /////////////////////////
    //function selecting and running
        
        
        //xprintf("");
        xprintf("select function type: \n1 - Constant | 2 - Linear | 3 - Quadratic | 4 - Rational\n");
        //cout<<"select function type: \n1 - Constant | 2 - Linear | 3 - Quadratic | 4 - Rational"<<endl;

        /*
        xprintf("asdfgh");
        //cout<<"asdfgh");
        same lenght!!!! :O
        */

        xscanf("%d", &FunctionType);
        //cin>>FunctionType;

        screen_clear();

            CenterX=DispX-1;
            CenterX=CenterX/2;

            CenterY=DispY-1;
            CenterY=CenterY/2;

        //ask for values
        switch (FunctionType)
            {
            case 1:
                xprintf("value: !full integers!\n");
                //cout<<"value: !full integers!"<<endl;
                xscanf("%d", &ValueA);
                //cin>>ValueA;
                break;

            case 2:
                xprintf("value a, b: !full integers!\n");
                //cout<<"value a, b: !full integers!"<<endl;
                xscanf("%d", &ValueA);
                xscanf("%d", &ValueP);
                //cin>>ValueA;
                //cin>>ValueP;
                break;
            
            case 3:
                xprintf("value a, b, c: !full integers!\n");
                //cout<<"value a, b, c: !full integers!"<<endl;
                xscanf("%d", &ValueA);
                xscanf("%d", &ValueP);
                xscanf("%d", &ValueQ);
                //cin>>ValueA;
                //cin>>ValueP;
                //cin>>ValueQ; 
                break;
            
            case 4:
                xprintf("value a, b, c: !full integers!\n");
                //cout<<"value a, b, c: !full integers!"<<endl;
                xscanf("%d", &ValueA);
                xscanf("%d", &ValueP);
                xscanf("%d", &ValueQ);
                //cin>>ValueA;
                //cin>>ValueP;
                //cin>>ValueQ;                
                break;
            }

        //run function and input result for given x into table
        while (POSX<=DispX-1){
            ValX=POSX-CenterX;
            ValY=ValX;                      //<- some stuff idk, i forgor what its doing
            
            //what function to use
            switch (FunctionType)
            {
            case 1:
                ValY=Constant(ValueA);
                break;

            case 2:
                ValY=Linear(ValY,ValueA,ValueP);
                break;
            
            case 3:
                ValY=Quadratic(ValY,ValueA,ValueP,ValueQ);
                break;
            
            case 4:              
                ValY=Rational(ValY,ValueA,ValueP,ValueQ);
                break;
            }

            POSY=CenterY-ValY;
            if(POSY>=DispY){
                POSX++;
            }                                   // <- same here, no idea what it does now
            else{
            D[POSX][POSY]=42;
            // 42 is *
            POSX++;
            }

        }
        
        POSY=0;
        POSX=0;
        
      //function  
     /////////////////////////////////   
    //drawing array on screen
    
        for(;POSY<=DispY-1;POSY++)
        {
            for(;POSX<=DispX-1;POSX++)
            {
                if(D[POSX][POSY]==0)
                   xprintf(" ");//empty space
                else
                   xprintf("%c", D[POSX][POSY]);
                   //cout<<D[POSX][POSY];
            }
            xprintf("\n");
            //cout<<endl;
            POSX=0;
        }
            POSY=0;
            POSX=0;


    //ask for running again//////////////////
    xprintf("run again? [Y/any]");
    xscanf("%d", &RunAgain);
    //cin>> RunAgain;
    if(RunAgain==121||RunAgain==89||RunAgain==49)
    {
        RunFlag=true;
        xprintf("clear graph(s)? [Y/any]");
        //cout<<"clear graph(s)? [Y/any]";
        xscanf("%d", &Clearing);
        //cin>> Clearing;

        if(Clearing==121||Clearing==89||Clearing==49)
        {
            for(;POSY<=26;POSY++)
            {
                for(;POSX<=78;POSX++)
                {
                    D[POSX][POSY]=0;
                }
                POSX=0;
            }
        }

        Clearing=0;
        POSY=0;
        POSX=0;
        
    }
    
    }
    return XANIN_OK;
} 