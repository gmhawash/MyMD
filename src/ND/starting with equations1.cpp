  #include <iostream>
  #include <string>
  #include <math.h>
  #include <fstream>
  #include <stdio.h>
  
  using namespace std;
int array[1048576][20],control[1048576][20], outarray[20], dif[20], cont[20],flip[1048576]; 
int qbit,i1,i2,i3,i4,i5,x,xx,y,b,bb,gatecount, column, row,couont;
  int main()
  {
    y=20;
    i3=0;
    gatecount=-1;
//=======================================================================
//=======================================================================    
// THE FOLLOWING SECTION GENERATES VECTORS FOR 20 BIT AND STORES THEM IN array[1048576][20] AND IN OUTPUT FILE output.txt
ofstream myfile;
  myfile.open ("output.txt");
    
      for ( row = 0; row < 1048576 ; row=row+2 ) {for (i1=0; i1<1; ++i1){array[row+i1][0]=0; array[row+1+i1][0]=1;}}    
      for ( row = 0; row < 1048576 ; row=row+4 ) {for (i1=0; i1<2; ++i1){array[row+i1][1]=0; array[row+2+i1][1]=1;}}  
      for ( row = 0; row < 1048576 ; row=row+8 ) {for (i1=0; i1<4; ++i1){array[row+i1][2]=0; array[row+4+i1][2]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+16 ) {for (i1=0; i1<8; ++i1){array[row+i1][3]=0; array[row+8+i1][3]=1;}}     
      for ( row = 0; row < 1048576 ; row=row+32 ){for (i1=0; i1<16; ++i1){array[row+i1][4]=0; array[row+16+i1][4]=1;}}     
      for ( row = 0; row < 1048576 ; row=row+64 ){for (i1=0; i1<32; ++i1){array[row+i1][5]=0; array[row+32+i1][5]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+128 ){for (i1=0; i1<64; ++i1){array[row+i1][6]=0; array[row+64+i1][6]=1;}}     
      for ( row = 0; row < 1048576 ; row=row+256 ){for (i1=0; i1<128; ++i1){array[row+i1][7]=0; array[row+128+i1][7]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+512 ){for (i1=0; i1<256; ++i1){array[row+i1][8]=0; array[row+256+i1][8]=1;}}     
      for ( row = 0; row < 1048576 ; row=row+1024 ){for (i1=0; i1<512; ++i1){array[row+i1][9]=0; array[row+512+i1][9]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+2048 ){for (i1=0; i1<1024; ++i1){array[row+i1][10]=0; array[row+1024+i1][10]=1;}}            
      for ( row = 0; row < 1048576 ; row=row+4096 ){for (i1=0; i1<2048; ++i1){array[row+i1][11]=0; array[row+2048+i1][11]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+8192 ){for (i1=0; i1<4096; ++i1){array[row+i1][12]=0; array[row+4096+i1][12]=1;}}
      for ( row = 0; row < 1048576 ; row=row+16384 ){for (i1=0; i1<8192; ++i1){array[row+i1][13]=0; array[row+8192+i1][13]=1;}}            
      for ( row = 0; row < 1048576 ; row=row+32768 ){for (i1=0; i1<16384; ++i1){array[row+i1][14]=0; array[row+16384+i1][14]=1;}}
      for ( row = 0; row < 1048576 ; row=row+65536 ){for (i1=0; i1<32768; ++i1){array[row+i1][15]=0; array[row+32768+i1][15]=1;}}               
      for ( row = 0; row < 1048576 ; row=row+131072 ){for (i1=0; i1<65536; ++i1){array[row+i1][16]=0; array[row+65536+i1][16]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+262144 ){for (i1=0; i1<131072; ++i1){array[row+i1][17]=0; array[row+131072+i1][17]=1;}}     
      for ( row = 0; row < 1048576 ; row=row+524288 ){for (i1=0; i1<262144; ++i1){array[row+i1][18]=0; array[row+262144+i1][18]=1;}}      
      for ( row = 0; row < 1048576 ; row=row+1048576 ){for (i1=0; i1<524288; ++i1){array[row+i1][19]=0; array[row+524288+i1][19]=1;}}                       
 
 
 system("PAUSE");     
//NOW PRINTING TO FILE               
//     for ( row = 0; row < 1048576 ; ++row){for ( column=0; column <20; ++column){myfile << array[row][column];}myfile << "\n";}          
//=======================================================================
//=======================================================================

for (row=0; row < 1048576; ++row)
{
outarray[0]=!array[row][1]; //cout << array[row][1] << array[row][14] << array[row][7] << outarray[0]<<"\n";
outarray[1]=array[row][5]; //cout << array[row][5] << array[row][17] << array[row][4] << outarray[1]<<"\n";;
outarray[2]=!array[row][7]; //cout << outarray[2]<<"\n";;
outarray[3]=array[row][0];
outarray[4]=array[row][15];
outarray[5]=array[row][9];
outarray[6]=!array[row][3];
outarray[7]=array[row][10];
outarray[8]=array[row][16];
outarray[9]=array[row][17];
outarray[10]=array[row][13];
outarray[11]=array[row][19];
outarray[12]=array[row][2];
outarray[13]=array[row][18];
outarray[14]=array[row][6];
outarray[15]=array[row][14];
outarray[16]=array[row][8];
outarray[17]=array[row][11];
outarray[18]=array[row][12];
outarray[19]=array[row][4];


for ( i1 = 0; i1 < 20 ;++ i1 )
{
     for (i4 = 0; i4 < gatecount ;++ i4 )
     {
    
         for (i5 = 0; i5 < 20 ;++ i5 ){if (control[i4][i5]==1 && outarray[i5]==0){break;}}    
         if (i5==20 && outarray[i1]==0){outarray[i1]=1;}
         else if (i5==20 && outarray[i1]==1){outarray[i1]=0;} 
     }
}



for ( i1 = 0; i1 < 20 ;++ i1 )
{ 
dif[i1]=0;    
if (outarray [i1]==1) {cont [i1]=1;} else {cont [i1]=0;}            //registering all control lines
if (outarray [i1] != array[row][i1]){dif[i1]=1;}    //registering if output does not equal input
}

for ( i1 = 0; i1 < 20 ;++ i1 )
{   
if (dif[i1]==1 && outarray[i1]==1) {cont[i1]=0; gatecount=gatecount+1;flip[gatecount]=i1;for (i3 = 0; i3 < 20 ;++ i3) {control[gatecount][i3]=cont[i3];}}            
else if (dif[i1]==1 && outarray[i1]==0) {gatecount=gatecount+1;for (i3 = 0; i3 < 20 ;++ i3) {control[gatecount][i3]=cont[i3];cont[i1]=1;}}
}

}




for (i4 = 0; i4 < gatecount ;++ i4 )
{
    cout << "gate "<< i4<<"\n";
       for ( i1 = 0; i1 < 20 ;++ i1 ){if (control[i4][i1]==1){cout << i1<<" ";}}
       cout <<"to flip "<< flip[i4];
    cout << "\n\n";

}


  
      
   myfile.close();
      system("PAUSE");
      return 0;
  }
