/*Omer BOZABA , 131044080
  2017 / 2018 , c++ Hw2
  Four in a line oyunu*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

void clearScreen(int satir = 30);
void printBoard(const char board[][20],const int size);
void initBoard (char board[][20],int size);

int getInput(string str);

/*SAVE - LOAD*/
void saveGame(const char board[][20], int& size ,const int& oyunModu,const bool& player,const string str);
void loadGame(char board[][20],int& size,int& hamleSayisi,int& oyunModu,bool& player,string str);
bool checkFileToLoad(string str);

/*Oyun , yapayZeka ve yapayZeka'nin dusunme fonksiyonu*/
void oyun(char board[][20],int size,int oyunModu,string input);
char yapayZeka(char board[][20],int size);
char oyunBiterMi(char board[][20],int size,char harf);

/*Oyun ici kontrol fonksiyonlari*/
int controls(char board[][20],int size,char harf,bool test);
int isLegal(char board[][20],int size, char coordinate);
int yatayKontrol(char board[][20],int size,char harf,bool test);
int dikeyKontrol(char board[][20],int size,char harf,bool test);
int solCaprazKontrol(char board[][20],int size,char harf,bool test);
int sagCaprazKontrol(char board[][20],int size,char harf,bool test);



int main(){

	string input;
    int boardSize=0,
        oyunModu=0,
        isLoad=0;	
    char board[20][20];

	clearScreen();
    do{	/*GIRIS EKRANI*/
		cout << "Kayitli bir oyunu yuklemek icin 'LOAD filename.txt'," << endl
			 << "Yeni bir oyun kurmak icin ise" << endl
		     << "Sadece 4 ile 20 arasinda bir cift sayi giriniz."<< endl
        	 << "Board size (4-20): ";
		/*input string olarak alinir*/        
		getline(cin , input);
		boardSize = getInput(input);
		if(boardSize == 3)/*input LOAD komutu ise*/
			isLoad = 1;
		if(isLoad != 1)
			if(boardSize <4 || boardSize >20 || boardSize % 2 != 0)	
				cerr << endl << "**ERROR: Hatali input degeri. " << endl;
    }while(!isLoad && (boardSize <4 || boardSize >20 || boardSize % 2 != 0));
    clearScreen();

	if(isLoad != 1){/*LOAD komutu ile bilgiler okunmamis ise*/
		cout << "Board Size : " << boardSize     << endl
			 << "Oyun modunu giriniz :"          << endl
			 << "\tOyuncu1 vs Oyuncu2 icin 'P',"  << endl
			 << "\tOyuncu vs PC  icin 'C',"      << endl
			 << "Kayitli bir oyunu yuklemek icin 'LOAD filename.txt'," << endl;
		do{
		/*Oyun Modu kontrolu burada yapiliyor*/
			cout <<"Oyun Modu (P-C): ";
			getline(cin , input);
			oyunModu = getInput(input);
			if(oyunModu == 3)/*OyunModu alinirken Load sorgusu*/
				isLoad = 1;
			if(oyunModu != 1 && oyunModu !=2 && isLoad !=1)
				cerr << "**ERROR: Hatali input degeri." << endl;
		}while(isLoad!=1 && (oyunModu != 1 && oyunModu !=2));
    }
    clearScreen();
	if(isLoad)/*LOAD komutu girilmis ise bilgiler oradan okunacak*/
		oyun(board,boardSize,oyunModu,input);
	else{/*yeni oyun kurulmus ise*/
		initBoard(board,boardSize);
    	printBoard (board, boardSize);
    	oyun(board,boardSize,oyunModu,"0"); /*Oyun baslatiliyor*/
    }
    return 0;
}
/*input parametresi, oyun degerleri bir dosyadan cekilmeyecek ise "0"
					 dosyadan cekilecek ise main'den filename gelecek*/
void oyun(char board[][20],int size,int oyunModu,string input){
    
	string str; /*hamle esnasinda karakter yerine string girilme hatasi icin*/
    bool player=1; 	/*1 user , 2 CPU veya user1,user2 */					 
	int  hamleSayisi=0,i=0,j=0,kazanan=0;
    char hamle , harf; 
    
    /*board full doldurulana kadar veya birisi kazanana kadar, biri kazanirsa
	  fonksiyon return ile sonlanir. */
	if(input != "0")/*Oyun bilgileri dosyadan cekilecek*/
		loadGame(board,size,hamleSayisi,oyunModu,player,input);
    while(hamleSayisi < size*size){/*Oyun motoru*/
        
        if(player){ /*player 1 , 'X' oynar*/
            harf = 'X';
            if(oyunModu==2)/*P vs P ise*/
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
        if(oyunModu==1 && player==0)/*yapay zeka calistirilir*/
            hamle = yapayZeka(board,size);
        else{/*Sira player'da*/ 
			/*tek karakter yerine string girilirse*/
		    getline(cin , str);
			if(str[1]=='\0'){/*string'de tek karakter girilmis ise*/		
				hamle = str[0];
				if(hamle>= 'A' && hamle <='Z')/* tolower input*/
					hamle = hamle-('A'-'a');
			}
			else{ /*input tek karakter degil ise*/
				/*SAVE - LOAD  veya gecersiz komut*/
				/*SAVE KOMUTU*/
				if(str[0]=='S' && str[1]=='A' && str[2]=='V' && str[3]=='E'){
					/*En kisa gecerli SAVE veya LOAD komutu karakter sayisi 10'dur,
					Ornegin  "LOAD a.txt", Bu nedenle str size > 9 olmali*/					
					if(str.size()>9){					
						saveGame(board,size,oyunModu,player,str);
						hamle = 'Z';/*Hatali hamle uyarisi vermesin diye buyuk z*/
					}
					else
						hamle = 'z';		
				}
				/*LOAD KOMUTU*/
				else if(str[0]=='L' && str[1]=='O' && str[2]=='A' && str[3]=='D'){
					
					if(checkFileToLoad(str)){
						loadGame(board,size,hamleSayisi,oyunModu,player,str);
						hamle = 'Z';/*Hatali hamle uyarisi vermesin diye buyuk z*/	
					}else
						hamle = 'z';/*Hata almak icin 'z'*/
				}
				else
					hamle = 'z';/*Hatali input uyarisi almak icin hamle = 'z' */	
			}/*string kosulu sonu*/
		}/*sira player'da son*/

		/*hamle oynanabilir mi ?  isLegal?  Oynanabilir ise bize satir 
		  return eder. i degiskenine atariz*/
        i=isLegal(board,size,hamle);
        j=hamle - 'a'; /*Orn. hamle 'b' ise 'b'-'a' = 1 olur sutun elde edilir*/
        
		if(i!=-1){ /*isLegal -1 dondurmemis ise hamle legal*/
            board[i][j] = harf;/*hamle boarda yazilir*/
            ++hamleSayisi;     /*hamle sayisi +1*/
            player = !player; /*sira diger oyuncuya verilir*/
            kazanan = controls(board,size,harf,0);/*bu hamle ile kazanildi mi?*/
            clearScreen();
            printBoard(board,size);
            if(kazanan != 0){/*0 degil ise oyun Kazanilmis*/
                if(kazanan==1){/*kazanan 1 ise oyun Moduna bakilir.Cpu or USER*/
                    if(oyunModu==2)/*PvsP'de 1.player kazandi*/
                        cout << endl << "Kazanan player 1" << endl;
                    else/*PvsCPU'da player kazandi*/
                        cout << endl << "Kazanan player" << endl;
                }
                else/*Kazanan 2.oynayan*/
					if(oyunModu==2)/*PvsP'de 2.player kazandi*/
                    	cout << endl << "Kazanan player 2" << endl;
					else/*PvsCPU'da CPU kazandi*/
						cout << endl << "Kazanan CPU" << endl;
                
				return; /*fonksiyondan cikar , main'de array free edilir*/
            }
        
        }else/*isLegal -1 return etmis ise , hatali hamle*/
			if(hamle!='Z')/*save veya load komutlari degil ise*/            
				cerr << endl << "**ERROR : Hatali input" << endl;
    }/*board full doldurulmus ve kazanan yok*/
    cout << endl << "OYUN BERABERLIK ILE SONLANDI" << endl;
    
}
int getInput(string str){

	/*input en az 4 karakter iceriyor ise 
	  SAVE veya LOAD icin*/
	if(str.size()>3){
		/*SAVE mi?*/
		if(str[0]=='S' && str[1]=='A' && str[2]=='V' && str[3]=='E'){
			cerr << "**ERROR : Henuz kaydedilecek bir oyun olusturulmadi"<< endl;
			return 0;
		}
		/*LOAD mi?*/
		else if(str[0]=='L' && str[1]=='O' && str[2]=='A' && str[3]=='D'){
			if(str.size()>9 && checkFileToLoad(str))
				return 3;
			else
				return 0;
		}
		else/*Ne hamle , ne "SAVE", ne de "LOAD" degil ise*/
			return 0;
	}
	/*input board size veya oyunModu'ndan birini iceriyor ise*/
	if(str == "4")  	 return 4;
	else if(str == "6")  return 6;
	else if(str == "8")  return 8;
	else if(str == "10") return 10;
	else if(str == "12") return 12;
	else if(str == "14") return 14;
	else if(str == "16") return 16;
	else if(str == "18") return 18;
	else if(str == "20") return 20;
	else if(str == "C" || str == "c") return 1;
	else if(str == "P" || str == "p") return 2;
	else return -1;
}
/*Secilen sutun hamle yapmak icin uygun mu?*/
int isLegal(char board[][20],int size, char coordinate){

    int i=0,j=0;
    
    j = coordinate - 'a';/*sutun hesaplama*/
	/*sutun degeri hatali girilmis ise*/
    if(j<0 || j>=size){
        return -1;
    }
	/*asagidan yukari dogru , o sutunde ilk bulunan bos yer return edilir*/
    for(i=size-1 ; i>=0 ; --i)
        if(board[i][j] == '.')
            return i;
    return -1;/*sutun dolu ise -1 return edilir*/
}

/*yapilan hamle ile oyun kazanildi mi? Tum kontrolleri calistirir.
  Parametredeki  harf  'X' veya 'O' icin mi bakilacak,
  parametredeki  test 0 ise kazanilan oyunda harfleri kucuk harf yapar,
				 test 1 ise yapay zeka hamle test ediyordur.
				 oynanmamis hamle oldugundan harfleri kucultmez*/
int controls(char board[][20],int size,char harf,bool test){
    
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
int yatayKontrol(char board[][20],int size,char harf,bool test){

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
int dikeyKontrol(char board[][20],int size,char harf,bool test){
    
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
int sagCaprazKontrol(char board[][20],int size,char harf,bool test){
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
int solCaprazKontrol(char board[][20],int size,char harf,bool test){
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
void clearScreen(int satir){
    int i=0;
    
    for(;i<satir;++i)
        cout << endl;

}
/*Board'a ilk deger atama*/
void initBoard (char board[][20],int size){

    int i=0,j=0;
    for(i=0; i<size; ++i)
        for(j=0; j<size; ++j)
            board[i][j]='.';       
}
/*Board'u ekrana yazdirma*/
void printBoard(const char board[][20],const int size){

    auto i=0;
	decltype (i) j;
	j = 0;
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
   -sirasi ile bos yerlere bakar , bu bos yerlere sirasi ile 'O' yazar , 
   bu bos yere 'O' yazildiktan sonra oyun kendisi icin tek hamlede bitirilebilir
   hale geliyor ise , oraya oynar.
  Eger bu durum da soz konusu degil ise random hamle yapar.*/
char yapayZeka(char board[][20],int size){
    
    int i=0,row,col;
    char hamle='a',hamle2;
	/*hamle2 ,  2 hamle sonrasini hesaplamak icin*/    

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
            ++hamle;        /*karakter artirma*/
        if(isLegal(board,size,hamle)!=-1)
            return hamle;
    }while(1);/*return edene kadar hamle arar*/
}
/*Yapay zekanin yapacagi bir hamle ile oyunu bitirebilip , bitiremeyeceginin
     testidir.Yapay zeka tarafindan defalarca calistirilir ve uygun hamle 
     tespit edilir.
  Harf olarak 'O' gonderilir ise , yapay zeka kendisinin oyunu bitirebilip 
     bitiremeyecegine bakar. Harf olarak 'X' gonderilir ise , yapay zeka 
     oyunu rakibinin bitirebilip bitiremeyecegine bakar. 
  Eger oyun tek hamlede bitirilebilir duruma gelmemis ise 'z' return edilir*/
char oyunBiterMi(char board[][20],int size,char harf){

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
void saveGame(const char board[][20],int& size,const int& oyunModu,const bool& player,string str){

	int i=0,j=0;
	/*Oyun motorunda str kontrol edildigi icin direk atamayi yaptim.
	dosya ".txt" uzantili mi diye bakabilmek icin sondaki 4 karakteri aliyoruz*/
	string dosyaUzantisi = &str[str.size()-4];
	if((dosyaUzantisi == ".TXT" || dosyaUzantisi == ".txt") && str.size()>9){
		
		ofstream fout;
		fout.open(&str[5]); /*str icindeki ilk 5 karakteri atiyoruz*/
							/*elde ettigimiz kisim filename.txt oluyor*/

		/*fout'a ilk bastigimiz satir , daha sonraki okumalarda dosyanin
		  programimiz tarafindan yazilip yazilmadigini bize gostermis olacak
		  Hatali yukleme dosyalarini boylece kontrol etmis oluyoruz*/
		fout << "Bu dosya 4 in a line oyunu save dosyasidir." << endl;
		fout << size << endl;
		fout << oyunModu << endl;
		fout << player << endl;
		for (i=0 ; i<size; ++i){
			for(j=0; j<size ; ++j)
				fout << board[i][j] <<" ";
			fout << endl;
		}
	
		fout.close();/*dosya kapama*/
		clearScreen();
		cout << "Save islemi basari ile tamamlandi."<<endl;
		printBoard(board,size);
	}
	else
		cerr << "ERROR : Filename Hatasi." << endl;

}
/*LOAD filename.txt komutundaki filename.txt taninabilir kayit dosyasi mi?*/
bool checkFileToLoad(string str){

	string dosyaUzantisi;
	
	/*En kisa gecerli LOAD komutu karakter sayisi 10'dur,Ornegin  "LOAD a.txt"
	  Bu nedenle str size > 9 olmali*/
	if(	str.size()>9 ) 
		dosyaUzantisi = &str[str.size()-4];/*saveGame()'deki gibi*/
	else{ 
		cerr << "**ERROR : 'LOAD filename.txt' seklinde giriniz" << endl;
		return 0;
	}
	if(dosyaUzantisi == ".txt" || dosyaUzantisi == ".TXT"){ /*dosya ".txt" uzantili mi?*/
		
		ifstream finp;
		finp.open(&str[5]); /*saveGame()'deki gibi*/
		if(finp.fail()){/*dosya acma hatasi*/
			cerr << "**ERROR : Okunacak dosya bulunamadi." << endl;
			return 0;
		}
		string forCheck;
		getline(finp,forCheck);	/*dosyadaki sifremiz olan ilk satiri okuyoruz*/
		/*dosyanin ilk satiri bizim sifre diye yazdigimiz cumle ise return 1*/
		if(forCheck == "Bu dosya 4 in a line oyunu save dosyasidir."){
			finp.close();/*dosya kapama*/
			return 1;		
		}
		else/*ilk satir bizim sifremiz degil ise*/
			cerr << "**ERROR : "<< &str[5] 
				 <<" dosyasi programa ait bir save dosyasi degil"<< endl;
	}
	else/*".txt" uzantisi bulunamadi ise*/ 
		cerr << "**ERROR : Hatali filename." << endl;
	return 0;
}
void loadGame(char board[][20],int& size,int& hamleSayisi,int& oyunModu,bool& player,string str){

	int i=0,j=0;
	
	string dosyaUzantisi = &str[str.size()-4];
	if((dosyaUzantisi == ".TXT" || dosyaUzantisi == ".txt") && str.size()>9){
		ifstream finp;
		finp.open(&str[5]);/*dosya acma*/
		if(finp.fail()){ /*dosya bulunamadi ise*/
			cerr << "**ERROR : Okunacak dosya bulunamadi." << endl;
			return;
		}
		string isOurFile;
		getline(finp,isOurFile);/*ilk satirdaki sifre okunur*/
		hamleSayisi =0;
		finp >> size ;
		finp >> oyunModu ;
		finp >> player ;
		for (i=0 ; i<size; ++i){
			for(j=0; j<size ; ++j){
				finp >> board[i][j];
				if(board[i][j] != '.')
					hamleSayisi +=1;/*hamleler sayilir*/
			}
		}

		finp.close(); /*dosya kapama*/
		clearScreen();
		
		/*success durumunda  oyun bilgileri print edilir*/
		cout <<"Load islemi basari ile tamamlandi."<<endl
			 <<"Oyun Bilgileri : "<< endl;
		  	 if(oyunModu==1)
		  		 cout << "Oyun Modu : Player vs CPU  - Sira Player'da. " << endl;
			 else{
				 cout << "Oyun Modu: Player1 vs Player2" << endl;
				 if(player)
					cout <<"Sira Player 1'de."<< endl;
			     else
				 	cout <<"Sira Player 2'de."<< endl;
			 }
		printBoard(board,size);
	}
	else /*Hatali filename*/
		cerr << "**ERROR : Hatali filename." << endl;
}
