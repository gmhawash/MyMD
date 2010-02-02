  #include <iostream>
  #include <string>
  #include <math.h>

  
  using namespace std;
  
int qbit,i1,x,y,b,bb, column, row,couont;
  int main()
  {
  cout << "Enter Number of bits ";
    cin >> qbit;  
    y=2;
    b=0;
    for (couont=0; couont < qbit-1; ++couont){y=y*2;} 
    cout << "Y=" << y;
    int array[y][qbit];
   cout << "\n";     
  

      for ( row = 0; row < y ; row=row+2 )  
      {
      array[row][0]=0;
      array[row+1][0]=1;   
      }
      
//=======================================================================      
      
      for ( row = 0; row < y ; row=row+4 )  
      {
      array[row][1]=0;
      array[row+1][1]=0;
      array[row+2][1]=1;    
      array[row+3][1]=1;
      }        
//=======================================================================     
      for ( row = 0; row < y ; row=row+2 )
      {   
      for (i1=0; i1<1; ++i1){array[row+i1][0]=0; array[row+1+i1][0]=1;}
      }    
//=======================================================================     
      for ( row = 0; row < y ; row=row+4 )
      {   
      for (i1=0; i1<2; ++i1){array[row+i1][1]=0; array[row+2+i1][1]=1;}
      }    
//=======================================================================           
      for ( row = 0; row < y ; row=row+8 )
      {   
      for (i1=0; i1<4; ++i1){array[row+i1][2]=0; array[row+4+i1][2]=1; cout << "array[" << row+i1 << "][2]=" << array[row+i1][2] <<"\n";}
      } 
//=======================================================================      
      for ( row = 0; row < y ; row=row+16 )
      {   
      for (i1=0; i1<8; ++i1){array[row+i1][3]=0; array[row+8+i1][3]=1;}
      } 
//=======================================================================     
      for ( row = 0; row < y ; row=row+32 )
      {   
      for (i1=0; i1<16; ++i1){array[row+i1][4]=0; array[row+16+i1][4]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+64 )
      {   
      for (i1=0; i1<32; ++i1){array[row+i1][5]=0; array[row+32+i1][5]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+128 )
      {   
      for (i1=0; i1<64; ++i1){array[row+i1][6]=0; array[row+64+i1][6]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+256 )
      {   
      for (i1=0; i1<128; ++i1){array[row+i1][7]=0; array[row+128+i1][7]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+512 )
      {   
      for (i1=0; i1<256; ++i1){array[row+i1][8]=0; array[row+256+i1][8]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+1024 )
      {   
      for (i1=0; i1<512; ++i1){array[row+i1][9]=0; array[row+512+i1][9]=1;}
      }        
//=======================================================================      
      for ( row = 0; row < y ; row=row+2048 )
      {   
      for (i1=0; i1<1024; ++i1){array[row+i1][10]=0; array[row+1024+i1][10]=1;}
      }        
//=======================================================================            
         
          
          
          //cout << array[row][column];   //cout << count;
          //cout << "array[" << row << "][" << column << "]=" << array[row][column];
          //cout << "\n";
          //cout << "\n";
          // x =((rand() % (10-4+1)) + 4);   // returns random number between 10 and 4
          //cout << "array[" << row << "][" << column << "]=" << array[row][column];
          //cout << "\n";
       
       
   
      
  
      system("PAUSE");
      return 0;
  }
