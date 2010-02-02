  #include <iostream>
  #include <string>
  #include <math.h>
  #include <fstream>
  #include <stdio.h>
  
  using namespace std;
int array[1048576][20], order[1048576], outarray[1048576][20], temparray[1048576][20], possibilities[1048576][32],DontCareLocation[1048576][20],DontCarePerRow[1048576],dontcareaddress[1048576][20],pwr[20], value[1048576],NumPossibilities[1048576],reserved[1048576],perm[1048576][25],hamming[20];
int row,i,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,reservedcounter, qbit,power, numPermInFile,ccvv,bq,ccdds,best;
FILE *in; 
ifstream is("incomplete.txt");
  int main()
  {
cout <<"Enter number of bits: ";
cin>>qbit;

power=2;
for (i6=1; i6<qbit; ++i6) {power=power*2;}      

ofstream file;      
ifstream issss("myperm.txt");


cout <<"Enter number of permutations in file: ";
cin>>numPermInFile;

for (i5=1; i5<numPermInFile; ++i5)
for (i6=1; i6<power; ++i6)
issss >> perm[i6][i5];
       

      

      

pwr[0]=1;
pwr[1]=2;
pwr[2]=4;
pwr[3]=8;
pwr[4]=16;
pwr[5]=32;
pwr[6]=64;
pwr[7]=128;
pwr[8]=256;
pwr[9]=512;
pwr[10]=1024;
pwr[11]=2048;
pwr[12]=4096;
pwr[13]=8192;
pwr[14]=16384;
pwr[15]=32768;
pwr[16]=65536;
pwr[17]=131072;
pwr[18]=262144;
pwr[19]=524288;


for ( i1 = 0; i1 < power ;++ i1 )
{
for (i=0; i < qbit; ++i){is >> temparray[i1][i]; cout << temparray[i1][i]; outarray[i1][i]=temparray[i1][i];}
cout<<"\n";
}





        for ( i1 = 0; i1 < power ;++ i1 )for ( i = 0; i < power ;++ i )possibilities[i1][i]=-1;
        
        for ( i1 = 0; i1 < power ;++ i1 ) {DontCarePerRow[i1]=0;NumPossibilities[i1]=0;}
        //=======================================================================
        //=======================================================================    
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
        //=======================================================================
        //=======================================================================
        
for ( i1 = 0; i1 < power ;++ i1 )for ( i = 0; i < qbit ;++ i ){if (outarray[i1][i]==9){DontCareLocation[i1][i]=1; dontcareaddress[i1][DontCarePerRow[i1]]=i; DontCarePerRow[i1]=DontCarePerRow[i1]+1; }else {DontCareLocation[i1][i]=0;}}

        
        
//for ( i1 = 0; i1 < power ;++ i1 )
//{
//                          for ( i = 0; i < qbit ;++ i )
//                          {
//                          cout << "DontCareLocation["<<i1<<"]["<<i<<"]= "<<DontCareLocation[i1][i]<<"\n";
//                        //for ( i2 = 0; i2 < DontCarePerRow[i1] ;++ i2 ){cout << "dontcareaddress["<<i2<<"]= "<<dontcareaddress[i2]<<"\n";
//                          }
//cout << "DontCarePerRow["<<i1<<"]= "<<DontCarePerRow[i1] ;
//cout <<"\n\n";
//}                              


for ( i1 = 0; i1 < power ;++ i1 ) for ( i2 = 0; i2 < DontCarePerRow[i1] ;++ i2 ){cout << "dontcareaddress["<<i1<<"]["<<i2<<"]= "<<dontcareaddress[i1][i2]<<"\n";}


for ( i1 = 0; i1 < power ;++ i1 )
    {
         value[i1]=0;
    for ( i2 = 0; i2 < qbit ;++ i2 )
        {
        if (outarray[i1][i2]!=9){value[i1]=value[i1]+(outarray[i1][i2] * pwr[i2]); }
        }
        cout << value[i1]<<"\n";        
    }            






for ( i1 = 0; i1 < power ;++ i1 )
    {
     if (DontCarePerRow[i1]==0){NumPossibilities[i1]=1; possibilities[i1][0]= value[i1];}
     if (DontCarePerRow[i1]==1){NumPossibilities[i1]=2; possibilities[i1][0]= value[i1];possibilities[i1][1]= value[i1]+pwr[dontcareaddress[i1][0]];}
     if (DontCarePerRow[i1]==2){NumPossibilities[i1]=4; possibilities[i1][0]= value[i1];possibilities[i1][1]= value[i1]+pwr[dontcareaddress[i1][0]];possibilities[i1][2]= value[i1]+pwr[dontcareaddress[i1][1]];possibilities[i1][3]= value[i1]+pwr[dontcareaddress[i1][0]]+pwr[dontcareaddress[i1][1]];}
     if (DontCarePerRow[i1]==3)
        {
                               NumPossibilities[i1]=8;
                               possibilities[i1][0]= value[i1];
                               possibilities[i1][1]= value[i1]+pwr[dontcareaddress[i1][0]];
                               possibilities[i1][2]= value[i1]+pwr[dontcareaddress[i1][1]];
                               possibilities[i1][3]= value[i1]+pwr[dontcareaddress[i1][2]];              
                               possibilities[i1][4]= value[i1]+pwr[dontcareaddress[i1][0]]+pwr[dontcareaddress[i1][1]];
                               possibilities[i1][5]= value[i1]+pwr[dontcareaddress[i1][0]]+pwr[dontcareaddress[i1][2]];
                               possibilities[i1][6]= value[i1]+pwr[dontcareaddress[i1][1]]+pwr[dontcareaddress[i1][2]];
                               possibilities[i1][7]= value[i1]+pwr[dontcareaddress[i1][0]]+pwr[dontcareaddress[i1][1]]+pwr[dontcareaddress[i1][2]];                              
        }



     if (DontCarePerRow[i1]==4)
        {
                               NumPossibilities[i1]=16;
                               for ( i8 = 0; i8 < 16 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]]);
        }


     if (DontCarePerRow[i1]==5)
        {
                               NumPossibilities[i1]=32;
                               for ( i8 = 0; i8 < 32 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]]);
        }

     if (DontCarePerRow[i1]==6)
        {
                               NumPossibilities[i1]=64;
                               for ( i8 = 0; i8 < 64 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]]);
        }

     if (DontCarePerRow[i1]==7)
        {
                               NumPossibilities[i1]=128;
                               for ( i8 = 0; i8 < 128 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]]);
        }


     if (DontCarePerRow[i1]==8)
        {
                               NumPossibilities[i1]=256;
                               for ( i8 = 0; i8 < 256 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]])+(array[i8][7]*pwr[dontcareaddress[i1][7]]);
        }


     if (DontCarePerRow[i1]==9)
        {
                               NumPossibilities[i1]=512;
                               for ( i8 = 0; i8 < 512 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]])+(array[i8][7]*pwr[dontcareaddress[i1][7]])+(array[i8][8]*pwr[dontcareaddress[i1][8]]);
        }


     if (DontCarePerRow[i1]==10)
        {
                               NumPossibilities[i1]=1024;
                               for ( i8 = 0; i8 < 1024 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]])+(array[i8][7]*pwr[dontcareaddress[i1][7]])+(array[i8][8]*pwr[dontcareaddress[i1][8]])+(array[i8][9]*pwr[dontcareaddress[i1][9]]);
        }

     if (DontCarePerRow[i1]==11)
        {
                               NumPossibilities[i1]=2048;
                               for ( i8 = 0; i8 < 2048 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]])+(array[i8][7]*pwr[dontcareaddress[i1][7]])+(array[i8][8]*pwr[dontcareaddress[i1][8]])+(array[i8][9]*pwr[dontcareaddress[i1][9]])+(array[i8][10]*pwr[dontcareaddress[i1][10]]);
        }

     if (DontCarePerRow[i1]==12)
        {
                               NumPossibilities[i1]=4096;
                               for ( i8 = 0; i8 < 4096 ;++ i8 )
                               possibilities[i1][i8]=value[i1]+(array[i8][0]*pwr[dontcareaddress[i1][0]])+(array[i8][1]*pwr[dontcareaddress[i1][1]])+(array[i8][2]*pwr[dontcareaddress[i1][2]])+(array[i8][3]*pwr[dontcareaddress[i1][3]])+(array[i8][4]*pwr[dontcareaddress[i1][4]])+(array[i8][5]*pwr[dontcareaddress[i1][5]])+(array[i8][6]*pwr[dontcareaddress[i1][6]])+(array[i8][7]*pwr[dontcareaddress[i1][7]])+(array[i8][8]*pwr[dontcareaddress[i1][8]])+(array[i8][9]*pwr[dontcareaddress[i1][9]])+(array[i8][10]*pwr[dontcareaddress[i1][10]])+(array[i8][11]*pwr[dontcareaddress[i1][11]]);
        }



     }



for ( i9 = 0; i9 < power ;++ i9 ) 
{  
            for ( i7 = 0; i7 < power ;++ i7 )
                        {
                        reservedcounter=0;
                        for ( i1 = 0; i1 < power ;++ i1 ) {if (NumPossibilities[i1]==1) {reserved[reservedcounter]=possibilities[i1][0];reservedcounter=reservedcounter+1;}}
//                        for ( i1 = 0; i1 < reservedcounter ;++ i1 ) cout << "reserved so far is "<< reserved[i1]<<"\n";
                        
if (reservedcounter==power) break;                        
                        
//                        for ( i1 = 0; i1 < power ;++ i1 )cout << "NumPossibilities["<<i1<<"] "<<NumPossibilities[i1]<<"\n";
                        /*cout << "reservedcounter= "<<reservedcounter<<"\n"*/;
                        
                        
                        for ( i2 = 0; i2 < reservedcounter ;++ i2 )
                            {
                            for ( i1 = 0; i1 < power ;++ i1 )
                                     {
                                     if (NumPossibilities[i1] != 1)
                                             {
                                             for (i3=0; i3<NumPossibilities[i1]; ++i3)
                                                 {
                                                 if (possibilities[i1][i3]==reserved[i2])
                                                    {
                      //                               for (i11=i3; i11<NumPossibilities[i1]; ++i11){possibilities[i1][i11]=possibilities[i1][i11+1];}                                    
                                                     possibilities[i1][i3]=possibilities[i1][NumPossibilities[i1]-1];
                                                    /*cout <<"before change : possibilities["<< i1 <<"]["<<i3<<"]= "<<possibilities[i1][i3]<< "after change: "<<possibilities[i1][i3+1]<<"\n"*/; for (i6=i3; i6<NumPossibilities[i1]; ++i6) {possibilities[i1][i6]=possibilities[i1][i6+1];}
                                                   // cout << "NumPossibilities["<<i1<<"] "<<NumPossibilities[i1]<<"\n";
                                                    NumPossibilities[i1]=NumPossibilities[i1]-1; break;
                                                   // cout << "NumPossibilities["<<i1<<"] "<<NumPossibilities[i1]<<"\n";
                                                    }
                                                 }
                                             }
                                     }
                            }
                        }
//            for ( i1 = 0; i1 < power ;++ i1 )cout << "NumPossibilities["<<i1<<"] "<<NumPossibilities[i1]<<"\n";
            
            
            /*
            for ( i1 = 0; i1 < power ;++ i1 )
                {
                 for ( i2 = 0; i2< pwr[DontCarePerRow[i1]] ;++ i2 )
                     {
            cout << "row" <<i1<<" "<< "possibility["<<i2<<"] " << possibilities[i1][i2]<<"\n";
            //              cout << "DontCarePerRow["<<i1<<"]= "<<DontCarePerRow[i1] <<"\n";
                     }     
            cout <<"\n";
                 }
            
            
            */
            
            
            
            ccdds=10000;
            for ( i1 = 0; i1 < power ;++ i1 )
                {
                 if (NumPossibilities[i1]!=1)
                    {
                    for ( i2 = 0; i2< NumPossibilities[i1] ;++ i2 )
                        {
                            hamming[i2]=0;     
                            for (i10=0; i10<qbit; ++i10)
                                {
                                if (array[possibilities[i1][i2]][i10]+ array[perm[i1][i10]][i10]==1) {hamming[i2]=hamming[i2]+1;}
                                }
                        }
                    for ( i2 = 0; i2< NumPossibilities[i1] ;++ i2 ){if (hamming[i2]< ccdds){ccdds=hamming[i2];best =i2;}}
                    possibilities[i1][0]=possibilities[i1][best];NumPossibilities[i1]=1;break;
                    }    
                }
cout <<"reservedcounter "<<reservedcounter<<"\n";
if (reservedcounter==power) {break;}        
}

//++++++++++++++++++++++++++++++++++++++++++++++



file.open("incomplete-output.txt");               

    
for ( i1 = 0; i1 < power ;++ i1 )
    {//file <<  "row "<<i1 <<"\n";
//     for ( i2 = 0; i2< NumPossibilities[i1] ;++ i2 )
//         {

file <<  possibilities[i1][0]<< " ";

//         }     
file <<"\n";
     }

file.close();








/*

for ( i1 = 0; i1 < power ;++ i1 )
    {
//    cout << "NumPossibilities["<<i1<<"] "<<NumPossibilities[i1];
     for ( i2 = 0; i2< NumPossibilities[i1] ;++ i2 )
         {
cout << "row" <<i1<<" "<< "possibility["<<i2<<"] " << possibilities[i1][i2]<<"\n";
//              cout << "DontCarePerRow["<<i1<<"]= "<<DontCarePerRow[i1] <<"\n";
         }     
cout <<"\n";
     }


*/




      system("PAUSE");
      return 0;
  }
