  #include <iostream>
  #include <string>
  #include <math.h>

  
  using namespace std;
int qbit, x, column, row;
  int main()
  {
  cout << "Enter Number of bits ";
    cin >> qbit;   
    int array[qbit][524288];
   cout << "\n";      
   for ( row = 0; row < qbit; ++row )                        //for ( int count = 0; count < pow(2,qbit); ++count )
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
