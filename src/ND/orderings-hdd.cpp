  #include <iostream>
  #include <string>
  #include <math.h>

  
  using namespace std;
int qbit, x,y, column, row,couont;
  int main()
  {
  cout << "Enter Number of bits ";
    cin >> qbit;  
    y=2;
    for (couont=0; couont < qbit-1; ++couont){y=y*2;} 
    cout << "Y=" << y;
    int array[qbit][y];
   cout << "\n";      
   for ( row = 0; row < pow (2,qbit) ; ++row )                        //for ( int count = 0; count < pow(2,qbit); ++count )
   {
      for ( column = 0; column < qbit; ++column )  
      {
          if (row == column){array[row][column]=1;} else {array[row][column]=0;}
          
          
          
          //cout << array[row][column];   //cout << count;
          //cout << "array[" << row << "][" << column << "]=" << array[row][column];
          //cout << "\n";
          //cout << "\n";
          // x =((rand() % (10-4+1)) + 4);   // returns random number between 10 and 4
          //cout << "array[" << row << "][" << column << "]=" << array[row][column];
          //cout << "\n";
       }
       
   }
  
      
  
      system("PAUSE");
      return 0;
  }
