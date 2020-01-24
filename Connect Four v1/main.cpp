/*
	Omer BOZABA 
  	Four in a line - CONNECT FOUR oyunu
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

void clearScreen();
void printBoard(char **board,int size);
void initBoard (char **board,int size);

/*Oyun , yapayZeka ve yapayZeka'nin dusunme fonksiyonu*/
void oyun(char **board,int size,int oyunModu);
char yapayZeka(char **board,int size);
char oyunBiterMi(char **board,int size,char harf);

/*Oyun ici kontrol fonksiyonlari*/
int controls(char **board,int size,char harf,bool test);
int isLegal(char **board,int size, char coordinate);
int yatayKontrol(char **board,int size,char harf,bool test);
int dikeyKontrol(char **board,int size,char harf,bool test);
int solCaprazKontrol(char **board,int size,char harf,bool test);
int sagCaprazKontrol(char **board,int size,char harf,bool test);


int main(){
    
	string secim;
    int i=0,j=0,
        boardSize=0,
        oyunModu=0,
        isFail=0;	
    char **board;

	clearScreen();
    do{
		/*youtube'dan alinti bu kisim. int deger yerine
		  karakter girilme hatasini engelliyor */
        do {
			cout << "Sadece 4 ile 20 arasinda bir cift sayi giriniz."<< endl;
            cout << "Board size (4-20): ";
            cin  >> boardSize;
            isFail = cin.fail();
            cin.clear();
            cin.ignore(100,'\n');
			if(isFail == true)
				cout << endl << "ERROR: Hatali input degeri. " << endl;
        }while(isFail == true);
        
        if(boardSize <4 || boardSize >20 || boardSize % 2 != 0)
            cout << endl << "ERROR: Hatali input degeri. " << endl;
        
    }while(boardSize <4 || boardSize >20 || boardSize % 2 != 0);
    clearScreen();
	cout << "Board Size : " << boardSize     << endl
         << "Oyun modunu giriniz :"          << endl
         << "\tOyuncu1 vs Oyuncu2 icin 'P'"  << endl
         << "\tOyuncu vs PC  icin 'C',"      << endl;
    do{
		/*Oyun Modu kontrolu burada yapiliyor*/
        cout <<"Oyun Modu (P-C): ";
        getline(cin , secim);
		if(secim == "P" || secim == "p")
 			oyunModu = 2;
		else if(secim == "C" || secim == "c")
			oyunModu = 1;
		else
			cout << "ERROR: Hatali input degeri." << endl;
    }while(oyunModu != 1 && oyunModu !=2);
    
	/*dinamik array*/
    board = (char**) malloc (boardSize * sizeof(char*));
    for (i=0;i<boardSize;++i)
        board[i] = (char*) malloc (boardSize * sizeof(char));
    
    initBoard(board,boardSize);
    clearScreen();
    printBoard (board, boardSize);
    oyun(board,boardSize,oyunModu); /*Oyun baslatiliyor*/
    
	/*dinamik array free ediliyor*/
    for(i=0 ; i<boardSize ; ++i)
        free(board[i]);
    free(board);
    return 0;
}
void oyun(char **board,int size,int oyunModu){
    
	string str; /*hamle esnasinda karakter yerine string girilme hatasi icin*/
    bool player=1; 	/*1 CPU , 2 user veya user1,user2 */					 
	int  hamleSayisi=0,i=0,j=0,kazanan=0;
    char hamle , harf; 
    
    /*board full doldurulana kadar veya birisi kazanana kadar, biri kazanirsa
	  fonksiyon return ile sonlanir. */
    while(hamleSayisi < size*size){
        
        if(player){ /*player 1 , 'X' oynar*/
            harf = 'X';
            if(oyunModu==2)
            	cout << "Player 1 hamle giriniz : ";
			else
				cout << "Player hamle giriniz : ";
        }
        else {      /*player 2, 'O' oynar*/
            harf = 'O';
            if(oyunModu==2)
                cout << "Player 2 hamle giriniz : ";
            else
                cout << "CPU oynuyor... : ";
        }
        /*Oyun modu CPU vs USER ise ve sira CPU'da ise*/
        if(oyunModu==1 && player==0)
            hamle = yapayZeka(board,size);
        else{ /*tek karakter yerine string girilirse, string'in bas harfi
				hamle kabul edilir*/
		    getline(cin , str);
			if(str[1]=='\0'){/*string'de tek karakter girilmis ise*/		
				hamle = str[0];
				if(hamle>= 'A' && hamle <='Z')/* tolower input*/
					hamle = hamle-('A'-'a');
			}
			else /*gecersiz deger atandi*/
				hamle = 'z';
		}
		/*hamle oynanabilir mi ?  isLegal?  Oynanabilir ise bize satir 
		  return eder. i degiskenine atariz*/
        i=isLegal(board,size,hamle);
        j=hamle - 'a'; /*Orn. hamle 'b' ise 'b'-'a' = 1 olur sutun elde edilir*/
        if(i!=-1){ /*isLegal -1 dondurmemis ise hamle legal*/
            board[i][j] = harf;
            ++hamleSayisi;
            player = !player; /*sira diger oyuncuya verilir*/
            kazanan = controls(board,size,harf,0);/*hamle ile kazanildi mi?*/
            clearScreen();
            printBoard(board,size);
            if(kazanan != 0){
                if(kazanan==1){/*kazanan 1 ise oyun Moduna bakilir.Cpu or USER*/
                    if(oyunModu==2)
                        cout << endl << "Kazanan player 1" << endl;
                    else
                        cout << endl << "Kazanan player" << endl;
                }
                else 
					if(oyunModu==2)
                    	cout << endl << "Kazanan player 2" << endl;
					else
						cout << endl << "Kazanan CPU" << endl;
                
				return; /*fonksiyondan cikar , main'de array free edilir*/
            }
        
        }else/*isLegal -1 return etmis ise , hatali hamle*/
            cout << endl << "Hatali input" << endl;
    }/*board full doldurulmus ve kazanan yok*/
    cout << endl << "OYUN BERABERLIK ILE SONLANDI" << endl;
    
}

/*Secilen sutun hamle yapmak icin uygun mu?*/
int isLegal(char **board,int size, char coordinate){

    int i=0,j=0;
    
    j = coordinate - 'a';
    if(j<0 || j>=size){
        return -1;
    }
    for(i=size-1 ; i>=0 ; --i)
        if(board[i][j] == '.')
            return i;
    return -1;
}

/*yapilan hamle ile oyun kazanildi mi? Tum kontrolleri calistirir.
  Parametredeki  harf  'X' veya 'O' icin mi bakilacak,
  parametredeki  test 0 ise kazanilan oyunda harfleri kucuk harf yapar,
				 test 1 ise yapay zeka hamle test ediyordur.
				 oynanmamis hamle oldugundan harfleri kucultmez*/
int controls(char **board,int size,char harf,bool test){
    
    int kazanan=0 , sonuc=0;
   
    sonuc = (yatayKontrol(board,size,harf,test)    ||
             dikeyKontrol(board,size,harf,test)    ||
             sagCaprazKontrol(board,size,harf,test)||
             solCaprazKontrol(board,size,harf,test)
            );
    if(sonuc)
        if(harf == 'X')
            kazanan=1;
        else
            kazanan=2;
    
    return kazanan;
}
/*yanyana 4 tane 'O' veya 'X' arar.*/
int yatayKontrol(char **board,int size,char harf,bool test){

    int i=0,j=0,find=0;
    
    for(i=0;i<size;++i)
        for(j=0;j+3<size;++j)
            if( board[i][j] == harf &&
                board[i][j+1] == harf &&
                board[i][j+2] == harf &&
                board[i][j+3] == harf    
              ){
                if(test==0){ /*kazanma sonrasi harfler kucultulur*/
                    board[i][j] = harf-('Z'-'z');
                    board[i][j+1] = harf-('Z'-'z');
                    board[i][j+2] = harf-('Z'-'z');
                    board[i][j+3] = harf-('Z'-'z');
                }
                find = 1;
            }
    return find;
}
/*dikey olarak 4 tane 'X' veya 'O' arar*/
int dikeyKontrol(char **board,int size,char harf,bool test){
    
    int i=0,j=0,find=0;
    
    for(j=0;j<size;++j)
        for(i=0;i+3<size;++i)
            if( board[i][j] == harf &&
                board[i+1][j] == harf &&
                board[i+2][j] == harf &&
                board[i+3][j] == harf    
              ){
                if(test==0){
                    board[i][j] = harf-('Z'-'z');
                    board[i+1][j] = harf-('Z'-'z');
                    board[i+2][j] = harf-('Z'-'z');
                    board[i+3][j] = harf-('Z'-'z');
                }
                find =1;
            }
    return find;
}
/*Sag asagi capraz olarak 4 tane 'X' veya 'O' arar*/
int sagCaprazKontrol(char **board,int size,char harf,bool test){
    int i=0,j=0,find=0;
    
    for(i=0;i+3<size;++i)
        for(j=0;j+3<size;++j)
            if( board[i][j] == harf &&
                board[i+1][j+1] == harf &&
                board[i+2][j+2] == harf &&
                board[i+3][j+3] == harf    
              ){
                if(test==0){
                    board[i][j] = harf-('Z'-'z');
                    board[i+1][j+1] = harf-('Z'-'z');
                    board[i+2][j+2] = harf-('Z'-'z');
                    board[i+3][j+3] = harf-('Z'-'z');
                }
                find = 1;
            }
    return find;
}
/*Sol asagi capraz olarak 4 tane 'X' veya 'O' arar*/
int solCaprazKontrol(char **board,int size,char harf,bool test){
    int i=0,j=0,find=0;
    
    for(i=size-4;i>=0;--i)
        for(j=size-1;j>=0;--j)
            if( board[i][j] == harf &&
                board[i+1][j-1] == harf &&
                board[i+2][j-2] == harf &&
                board[i+3][j-3] == harf    
              ){
                if(test==0){
                    board[i][j] = harf-('Z'-'z');
                    board[i+1][j-1] = harf-('Z'-'z');
                    board[i+2][j-2] = harf-('Z'-'z');
                    board[i+3][j-3] = harf-('Z'-'z');
                }
                find = 1;
            }
    return find;
}
/*Board daha sade goruntulensin diye ekranda satir atlar*/
void clearScreen(){
    int i=0;
    
    for(;i<30;++i)
        cout << endl;

}
/*Board'a ilk deger atama*/
void initBoard (char **board,int size){
    
    int i=0,j=0;
    for(i=0; i<size; ++i)
        for(j=0; j<size; ++j)
            board[i][j]='.';       
}
/*Board'u ekrana yazdirma*/
void printBoard(char **board,int size){

    int i=0,j=0;
    for (i=0; i < size ; ++i)
        cout << static_cast<char>('a'+i) << " ";
    cout << endl;
    for(i=0 ; i<size; ++i){
        for(j=0; j<size; ++j)
            cout << board[i][j]<<" ";
        cout << endl;
    }
}
/*Yapay zeka once kendisi tek hamlede oyunu bitirebiliyor mu diye kontrol eder,
     oyunu bitirebiliyor ise o noktaya hamlede bulunur.
  Eger kendisi tek hamlede bitiremiyor ise , rakibi tek hamlede bitirebilecek
      durumda ise , o noktaya hamle yapar.
  Bu iki durum da soz konusu degil ise , 2 hamle sonrasini hesaplar.
  Bunu da soyle yapar ,
   -sirasi ile bos yerlere bakar , bu bos yerlere sirasi ile 'X' yazar , 
   bu bos yere 'X' yazildiktan sonra oyun kendisi icin tek hamlede bitirilebilir
   hale geliyor ise , oraya oynar.
  Eger bu durum da soz konusu degil ise random hamle yapar.*/
char yapayZeka(char **board,int size){
    
    int i=0,row,col;
    
    char hamle='a',hamle2;
    
	/*CPU oyunu tek hamlede bitirebilir mi?*/
    hamle = oyunBiterMi(board,size, 'O');
    if(hamle != 'z')
        return hamle;

    /*Oyuncu oyunu tek hamlede bitirebilir mi?*/
    hamle = 'a';
    hamle = oyunBiterMi(board,size, 'X');
    if(hamle != 'z')
        return hamle;
    
    /*2 hamle İlerisi için atak hesaplaması*/
	hamle = 'a';
	for(i=0 ; i<size; ++i){
		row=isLegal(board,size,hamle);
		if(row!=-1){
		col=hamle - 'a';
		board[row][col] = 'O';/*Eger buraya hamle yaparsa*/
		hamle2 = oyunBiterMi(board,size, 'O');/*burada oyun tek hamlelik mi olur?*/
		board[row][col] = '.';                /*Oyle ise hamle'yi return eder*/
		if(hamle2 != 'z')
			return hamle;
		}
	++hamle;  
	}
  
	/*2 hamle sonrasinda oyun kazanilabilecek durumda
	  degil ise random bir hamle yapar*/
    srand(time(NULL));
   
    do{
        col = rand()% size;
        hamle = 'a';
        for (i=0 ;i<col; ++i)
            ++hamle;
        if(isLegal(board,size,hamle)!=-1)
            return hamle;
    }while(1);
}
/*Yapay zekanin yapacagi bir hamle ile oyunu bitirebilip , bitiremeyeceginin
     testidir.Yapay zeka tarafindan defalarca calistirilir ve uygun hamle 
     tespit edilir.
  Harf olarak 'X' gonderilir ise , yapay zeka kendisinin oyunu bitirebilip 
     bitiremeyecegine bakar. Harf olarak 'O' gonderilir ise , yapay zeka 
     oyunu rakibinin bitirebilip bitiremeyecegine bakar. 
  Eger oyun tek hamlede bitirilebilir duruma gelmemis ise 'z' return edilir*/
char oyunBiterMi(char **board,int size,char harf){

    int i=0,j=0,row,col,bitis=1;/*bitis 1  CPU , bitis 2  user icin kontrol*/
    
    char hamle='a';
   
    if (harf=='O')/*harf 'X' ise bitis=1 , harf 'O' ise bitis=2*/
        bitis = 2;
    
    for(i=0; i<size ;++i){
        row=isLegal(board,size,hamle);
        if(row!=-1){
            col=hamle - 'a';
            board[row][col] = harf;
            if(controls(board,size,harf,1)==bitis){
                board[row][col]='.';
                return hamle;
            }
            else
                board[row][col]='.';
        }
        ++hamle;
    }
    return 'z';
}
