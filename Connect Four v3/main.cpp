/*Omer BOZABA , 131044080
  2017 / 2018 , c++ Hw4
  Four in a line oyunu*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;


class ConnectFour{
	
	private:
		class Cell{
			public:
				/*constrctrs.*/
				Cell(){};
				Cell(int x,int y,char c){
					row=x;
					col=y;
					hucre=c;
				}
				/*setter*/
				void set(char c){ hucre=c; };
				void setRow(int x){ row = x;}
				void setCol(int x){ col = x;}
								
				/*getters*/
				inline int getRow()const{ return row; }
				inline int getCol()const{ return col; }
				char getValue()const	{return hucre;}
				bool operator==(const Cell& obj)const;				
				Cell operator++();
				Cell operator++(int);
				Cell operator--();
				Cell operator--(int);
				
				/*'<<' ve '>>' youtube'dan
					https://www.youtube.com/watch?v=TwW2Nl45FvE
				*/
				friend ostream& operator <<(ostream& output,Cell& obj);
				friend istream& operator >>(istream& input,Cell& obj);

			private:
				int row;
				int col;
				char hucre;
		};
		Cell **gameCells;
		int yukseklik,genislik,end,oyunModu,hamleSayisi;
		bool player;
		static int livingCell;
		
		/*_______PRIVATE FUNCTIONS_________*/
		/*compare*/
		int whichIsBigger(ConnectFour & obje)const;
		char oyunBiterMi(char harf);
		void play(Cell& hucre,char hamle){hucre.set(hamle);}
		/*__controls*/
		int yatayKontrol(char harf,bool test);
		int dikeyKontrol(char harf,bool test);
		int solCaprazKontrol(char harf,bool test);
		int sagCaprazKontrol(char harf,bool test);

	

	public:
		/*3 constrctrs*/
		ConnectFour():ConnectFour(5,5){}/*default 5x5*/
		ConnectFour(int dikey,int yatay);
		ConnectFour(int dikey,int yatay,int gameMod);
		ConnectFour(const string fileName);
	
		/*Destructor*/
		~ConnectFour();
		
		/*operators*/
		ConnectFour& operator=(const ConnectFour& obj);
		bool operator== (const ConnectFour& obj)const;
		bool operator!= (const ConnectFour& obj)const;
		

		/*PUBLIC FUNCTIONS*/
		/*__SAVE - LOAD*/
		void saveGame(string str)const;
		void loadGame(string str);
		/*__print*/
		void printBoard()const;
		
		
		friend ostream& operator <<(ostream& output,Cell& obj);
		friend istream& operator >>(istream& input,Cell& obj);
		
		/*__play fonksiyonlari*/
		void playGame(string input="0");
		char play();/*Yapay zeka - oynanacak hamleyi hesaplar , oynar*/
		/*controls*/
		int controls(char harf,bool test);
		int isLegal(char coordinate)const;
		/*__getters*/
		inline int currentWidth()const     {	return genislik;			}
		inline int currentHeight()const    {	return yukseklik;			}
		inline int getOyunModu()const      {	return oyunModu;			}
		inline int getHamleSayisi()const   {	return hamleSayisi;			}
		inline bool getPlayer()const       {	return player; 				}
		inline bool isGameEnd()const       {	return end;					}
		static int getLivingCell()   	   {	return livingCell;			}
		/*__setters*/
		void setOyunModu(int mod) 		{	oyunModu = mod;			 }
		void setHamleSayisi(int hS) 	{	hamleSayisi = hS; 		 }
		void setPlayer(bool pl)			{	player = pl;			 }
		void setGameEnd()				{	end = 1; 				 }
		void yaz(int i,int j,char harf) { gameCells[i][j].set(harf); }
		void artirLivingCell()			{ ++livingCell;				 }
};

int ConnectFour::livingCell = 0;

void clearScreen();
int getInput(string str);
bool checkFileToLoad(string str);/*dosya bu programin kayit dosyasi mi?*/
void multiGame();

int main(){

	string input , str;

	/*S or M*/
	do{
		cout << "Tekli Oyun icin ('S') , Coklu Oyun icin ('M') : ";
		getline(cin, input);
		if(cin.eof()){
			cout<<"End of File"<<endl;
			return 0;
		}
	}while(input!= "S" && input !="M");

	/*M ise*/
	if(input=="M"){
		multiGame();
		return 0;
	}
	/*S ise string filename alir*/
	getline(cin, str);
	if(cin.eof()){ /*input yok ise bosa beklemesin*/
		cout<<"End of File"<<endl;
		return 0;
	}
	ConnectFour obj(str);/*obje olusturulur*/
	obj.printBoard(); /* print edilir*/
	obj.playGame(); /*playGame fonksiyonu cagrilir*/
	return 0;
}


/*Multi Game fonksiyonu*/
void multiGame(){
	/*variables*/
	ConnectFour *games;
	int i,j,en,boy,mod,isFail=0,secim,kazanan=0,oyunModu;
	string input,str;
	char harf,hamle;
	bool player;
	
	games = new ConnectFour[5];
	/*5 obje olusturuluyor*/
	for(i=0;i<5;++i){
		
		cout << endl;
		cout <<"Obje"<<i+1<<":"<<endl;
		getline(cin,input);
		if(cin.eof()){
			cout<<"End of File"<<endl;
			return;
		}
		games[i] = ConnectFour(input);
		/*kendi '=' overloadimiz
		  deep copy */
		getline(cin,input);
		if(cin.eof()){
			cout<<"End of File"<<endl;
			return;
		}
		oyunModu = getInput(input);
		games[i].setOyunModu(oyunModu);

	}
	/*butun oyunlar bitmedikce oyun devam eder*/
	clearScreen();
	while( !games[0].isGameEnd() ||  !games[1].isGameEnd() ||
		   !games[2].isGameEnd() ||  !games[3].isGameEnd() ||
		   				!games[4].isGameEnd() 
		)
	{
		
		do{/*obje secimi*/
			cout <<"Obje Seciniz : ";
			cin >> secim;
			if(cin.eof()){
				cout <<"\nINPUT DOSYA SONU"<<endl;		
				return;
			}
			isFail = cin.fail() || secim<1 || secim>5;
       	 	cin.clear();
        	cin.ignore(100,'\n');
			if(isFail == true)
				cerr << endl << "ERROR: Hatali input degeri. " << endl;
		}while(isFail);
		
		--secim;  /*Ornek input : 1.obje  0.index */
		
		if(games[secim].isGameEnd()!=0)
			cout << "Bu oyun daha once bitirilmis." << endl;
		else{
			
			oyunModu = games[secim].getOyunModu();
			player   = games[secim].getPlayer();
			
			int k = 2;/*en az 2 hamle yapilsin*/
			
			/*eger board'a load ile onceden tek hamle yapilmis halde yuklenmis ise
			  Mesela player 1 oynamistir , sira player2'dedir*/
			if(games[secim].getPlayer()==0)
				--k;
			
			/*k kadar hamle yapilacak , ve eger k 0 olmadan tek hamlede oyun biterse bitirir*/
			while(k>0 && !games[secim].isGameEnd()){
				clearScreen();
				games[secim].printBoard();
				if(games[secim].getPlayer()){ /*player 1 , 'X' oynar*/
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
	          	  hamle = games[secim].play();
	   	    	else{/*Sira player'da*/ 
					/*tek karakter yerine string girilirse*/
				    getline(cin , str);
					if(cin.eof()){
						cout <<"\nINPUT DOSYA SONU"<<endl;		
						return;
					}
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
								games[secim].saveGame(str);
								hamle = '1';/*Hatali hamle uyarisi vermesin diye '1'*/
								++k;
							}
							else
								hamle = '0';		
						}
						/*LOAD KOMUTU*/
						else if(str[0]=='L' && str[1]=='O' && str[2]=='A' && str[3]=='D'){
							
							if(checkFileToLoad(str)){
								games[secim].loadGame(str);
								hamle = '1';/*Hatali hamle uyarisi vermesin diye '1''*/
								++k;	
							}else
								hamle = '0';/*Hata almak icin '0'*/
						}
						else
							hamle = '0';/*Hatali input uyarisi almak icin hamle = '0' */	
					}/*string kosulu sonu*/
				}/*sira player'da son*/
	
					/*hamle oynanabilir mi ?  isLegal?  Oynanabilir ise bize satir 
				  return eder. i degiskenine atariz*/
		        i=games[secim].isLegal(hamle);
	    	    j=hamle - 'a'; /*Orn. hamle 'b' ise 'b'-'a' = 1 olur sutun elde edilir*/
		        
				if(i!=-1){ /*isLegal -1 dondurmemis ise hamle legal*/
		            
					games[secim].yaz(i,j,harf);/*hamle boarda yazilir*/
		            games[secim].setHamleSayisi(games[secim].getHamleSayisi()+1); /*hamle sayisi +1*/
		            games[secim].artirLivingCell();
		            player = !player; /*sira diger oyuncuya verilir*/
		            games[secim].setPlayer(!games[secim].getPlayer());
		            kazanan = games[secim].controls(harf,0);/*bu hamle ile kazanildi mi?*/
		            clearScreen();
					cout << "OBJE " << secim+1<<endl;
		            games[secim].printBoard();
					
					
		            
					cout <<"LivingCell: "<< games[secim].getLivingCell() << endl;
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
		                
		                games[secim].setGameEnd();
						
		            }
	        
		        }else{/*isLegal -1 return etmis ise , hatali hamle*/
					if(hamle!='1')/*save veya load komutlari degil ise*/            
						cerr << endl << "**ERROR : Hatali input" << endl;
					++k;
				}
				--k;
				
			}
		}/*butun board dolmus ise, bu oyun draw olmustur*/
		if(games[secim].getHamleSayisi() >= games[secim].currentHeight() * games[secim].currentWidth()){
			cout << "Draw"<<endl;
			games[secim].setGameEnd();
		}
		if(games[0]==games[1])
			cout << "\nobj1 ve obj2 esit 2 objedir."<<endl;
		else
			cout << "\nobj1 ve obj2 esit 2 obje degildir."<<endl;
	}
	delete[] games;
}

ostream& operator << (ostream& output,ConnectFour::Cell& obj){
/*degerleri ekrana basar*/

	output  << "Row: "<<obj.getRow()<<endl
			<< "Col: "<<obj.getCol()<<endl
			<< "Value: "<<obj.getValue()<<endl;
	return output;
}
istream& operator >> (istream& input,ConnectFour::Cell& obj){

/*sirasiyla degerleri alir ve set fonksiyonlari ile yazar*/
	int x,y;
	char c;
	input >> x >> y >> c;
	obj.setRow(x);
	obj.setCol(y);
	obj.set(c);
	return input;
}

ConnectFour& ConnectFour::operator= (const ConnectFour& obj){
	
	if(this != &obj){/*objenin kendisi degil ise*/
		int i=0,j=0;
		
		/*eski gameCell silinir*/
		for(i=0 ; i<yukseklik;++i)
			delete[] gameCells[i];
		delete[] gameCells;
		
		/*degerleri kopyalama*/	
		yukseklik = obj.yukseklik;
		genislik = obj.genislik;
		end = obj.end;
		oyunModu = obj.oyunModu;
		hamleSayisi = obj.hamleSayisi;
		player = obj.player;
	
		/*deep copy*/
		gameCells = new Cell*[yukseklik];
		for(i=0;i<yukseklik;++i)
			gameCells[i] = new Cell[genislik];

		for(i=0;i<yukseklik;++i){
			for(j=0;j<genislik;++j)
				gameCells[i][j] = obj.gameCells[i][j];
		}

	}


	return *this;
}
bool ConnectFour::operator== (const ConnectFour& obj)const{
	
	if(this == &obj)
		return 1;
	/*tum degiskenleri ayni degerde ise 
	  ve gameCell'lerinin de degerleri ayni ise return 1
	  farkli herhangi bir durumda return 0*/
	if( yukseklik==obj.yukseklik &&
		genislik ==obj.genislik  &&
		oyunModu ==obj.oyunModu  && 
		hamleSayisi==obj.hamleSayisi)
	{
		for(int i=0;i<yukseklik;++i)
			for(int j=0; j<genislik;++j)
				if(!(gameCells[i][j] == obj.gameCells[i][j]))
					return 0;
		return 1;
	}
	
	return 0;
}

bool ConnectFour::operator!= (const ConnectFour& obj)const{
	
	if(this == &obj)
		return 0;
	/*tum degiskenleri ayni degerde ise 
	  ve gameCell'lerinin de degerleri ayni ise return 0
	  farkli herhangi bir durumda return 1*/
	if( yukseklik==obj.yukseklik &&
		genislik ==obj.genislik  &&
		oyunModu ==obj.oyunModu  && 
		hamleSayisi==obj.hamleSayisi)
	{
		for(int i=0;i<yukseklik;++i)
			for(int j=0; j<genislik;++j)
				if(!(gameCells[i][j] == obj.gameCells[i][j]))
					return 1;
		return 0;
	}
	else
		return 1;
}


bool ConnectFour::Cell:: operator==(const Cell& obj)const{
	return (hucre == obj.hucre) && (col == obj.col) && (row == obj.row);
}

/*-----------------------------------------
  pre and post increment/decrement for Cell
-------------------------------------------
  user 1   'A' oynuyor olsun,
  user 2   'B' oynuyor olsun,
  computer 'C' oynuyor olsun.

	"++cell" ve "cell++" ifadeleri , 
		cell'in degeri empty ise   'A' degerine ,
		cell'in degeri  'A'  ise   'B' degerine ,
		cell'in degeri  'B'  ise   'C' degerine , donusturecektir.
  
	"--cell" ve "cell--" ifadeleri ise,
		cell'in degeri 'C' ise 'B'   degerine ,
		cell'in degeri 'B' ise 'A'   degerine , 
		cell'in degeri 'A' ise empty degerine , donusturecektir.


	parametresiz operatorler , value degerini degistirip kendilerini return
	ederler,  parametreli olanlar ise ,  once kendilerinin kopyasini alir,
	sonra kendi degerlerini degistirir ve kopyayi return ederler.
*/
ConnectFour::Cell ConnectFour::Cell::operator++(){
	
	if(hucre == ' ')
		hucre = 'A';
	else if(hucre == 'A')
		hucre = 'B';
	else if(hucre == 'B')
		hucre = 'C';
	else if(hucre == 'C')
		hucre = ' ';
	
	return *this;	
}
ConnectFour::Cell ConnectFour::Cell::operator++(int){

	Cell temp = *this;
	
	if(hucre == ' ')
		hucre = 'A';
	else if(hucre == 'A')
		hucre = 'B';
	else if(hucre == 'B')
		hucre = 'C';
	else if(hucre == 'C')
		hucre = ' ';

	return temp;

}
ConnectFour::Cell ConnectFour::Cell::operator--(){

	if(hucre == 'A')
		hucre = ' ';
	else if(hucre == 'B')
		hucre = 'A';
	else if(hucre == 'C')
		hucre = 'B';
	else if(hucre == ' ')
		hucre = 'C';

	return *this;
}
ConnectFour::Cell ConnectFour::Cell::operator--(int){

	Cell temp = *this;

	if(hucre == 'A')
		hucre = ' ';
	else if(hucre == 'B')
		hucre = 'A';
	else if(hucre == 'C')
		hucre = 'B';
	else if(hucre == ' ')
		hucre = 'C';

	return temp;
}



ConnectFour::ConnectFour(const string fileName){

	hamleSayisi=0;
	player=1;
	end =0;

	
	ifstream finp;
	
	finp.open(&fileName[0]);/*dosya acma*/
	if(finp.fail()){ /*dosya bulunamadi ise*/
		cerr << "**ERROR : Okunacak dosya bulunamadi.Constructor exit." << endl;
		exit(1);
	}

	string str;
	int en=0,boy=0,satir=0,i,j,max=0;
	
	while(!finp.eof()){

		getline(finp,str);
		if(str.size() > max)
			max = str.size();
		++satir;	
	}
	--satir; /*1 fazla okunur, sondaki '\n' sebebi ile*/
	yukseklik = satir;
	genislik = max;
	finp.close();
	finp.open(&fileName[0]); /*dosyanin basina donus*/

	gameCells = new Cell*[yukseklik];
	for(i=0;i<yukseklik;++i)
		gameCells[i] = new Cell[genislik];

	for(i=0;i<yukseklik;++i){
		for(j=0;j<genislik;++j)
			gameCells[i][j] = Cell(i,j,' ');
	}
	i=0;
	while(!finp.eof()){

		getline(finp,str);
	
		for(j=0;j<str.size();++j)
			if(str[j]=='*')
				gameCells[i][j].set('.');
		++i;
	}
	finp.close();
}

ConnectFour::ConnectFour(int dikey,int yatay,int gameMod){
	int i,j;
	
	hamleSayisi=0;
	yukseklik = dikey;
	genislik = yatay;
	oyunModu = gameMod;
	player=1;
	end =0;
	
	gameCells = new Cell*[yukseklik];
	
	for(i=0; i<dikey; ++i){
		Cell* hucre = new Cell[genislik];
	
		for(j=0;j<genislik;++j){
			Cell hucreTek(i,j,'.');
			hucre[j] = hucreTek;
		}
		gameCells[i]=new Cell[genislik];
		for(int k=0; k<genislik; ++k)
			gameCells[i][k] = hucre[k];
	}
}

ConnectFour::ConnectFour(int dikey,int yatay){
	int i,j;
	hamleSayisi=0;
	yukseklik = dikey;
	genislik = yatay;
	oyunModu = 1;
	player=1;
	end= 0;
	
	gameCells = new Cell*[yukseklik];
	
	for(i=0; i<dikey; ++i){
		Cell* hucre = new Cell[genislik];
	
		for(j=0;j<genislik;++j){
			Cell hucreTek(i,j,'.');
			hucre[j] = hucreTek;
		}
		gameCells[i]=new Cell[genislik];
		for(int k=0; k<genislik; ++k)
			gameCells[i][k] = hucre[k];
	}
}
/*destructor of ConnectFour*/
ConnectFour::~ConnectFour(){
	int i=0;
	
	for(i=0;i<yukseklik;++i)
		delete[] gameCells[i];
	delete[] gameCells;
}



/*input parametresi, oyun degerleri bir dosyadan cekilmeyecek ise "0"
					 dosyadan cekilecek ise main'den filename gelecek*/
void ConnectFour::playGame(string input){
    
	string str; /*hamle esnasinda karakter yerine string girilme hatasi icin*/
    	/*1 user , 2 CPU veya user1,user2 */					 
	int  i=0,j=0,kazanan=0;
    char hamle , harf; 

	getline(cin ,str);
	if(cin.eof()){
		cout<<"End of File"<<endl;
		return;
	}
	oyunModu = getInput(str);
    /*board full doldurulana kadar veya birisi kazanana kadar, biri kazanirsa
	  fonksiyon return ile sonlanir. */
	if(input != "0")/*Oyun bilgileri dosyadan cekilecek*/
		loadGame(input);
    while(hamleSayisi < currentHeight()*currentWidth()){/*Oyun motoru*/
        
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
            hamle = play();
        else{/*Sira player'da*/ 
			/*tek karakter yerine string girilirse*/
		    getline(cin , str);
			if(cin.eof()){
				cout << " \ninput -> Dosya sonu"<< endl;
				return;
			}
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
						saveGame(str);
						hamle = '1';/*Hatali hamle uyarisi vermesin diye buyuk '1'*/
					}
					else
						hamle = '0';		
				}
				/*LOAD KOMUTU*/
				else if(str[0]=='L' && str[1]=='O' && str[2]=='A' && str[3]=='D'){
					
					if(checkFileToLoad(str)){
						loadGame(str);
						hamle = '1';/*Hatali hamle uyarisi vermesin diye buyuk '1''*/	
					}else
						hamle = '0';/*Hata almak icin '0'*/
				}
				else
					hamle = '0';/*Hatali input uyarisi almak icin hamle = '0' */	
			}/*string kosulu sonu*/
		}/*sira player'da son*/

		/*hamle oynanabilir mi ?  isLegal?  Oynanabilir ise bize satir 
		  return eder. i degiskenine atariz*/
        i=isLegal(hamle);
        j=hamle - 'a'; /*Orn. hamle 'b' ise 'b'-'a' = 1 olur sutun elde edilir*/
        
		if(i!=-1){ /*isLegal -1 dondurmemis ise hamle legal*/
            play(gameCells[i][j],harf);/*hamle boarda yazilir*/
            ++hamleSayisi;     /*hamle sayisi +1*/
            artirLivingCell();
            player = !player; /*sira diger oyuncuya verilir*/
            kazanan = controls(harf,0);/*bu hamle ile kazanildi mi?*/
            clearScreen();
            printBoard();
            cout <<"LivingCell: "<< getLivingCell() << endl;
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
                
                setGameEnd();
				return; /*fonksiyondan cikar , main'de array free edilir*/
            }
        
        }else/*isLegal -1 return etmis ise , hatali hamle*/
			if(hamle!='1')/*save veya load komutlari degil ise*/            
				cerr << endl << "**ERROR : Hatali input" << endl;
    }/*board full doldurulmus ve kazanan yok*/
    cout << endl << "OYUN BERABERLIK ILE SONLANDI" << endl;
    setGameEnd();
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
	else if(str == "C" || str == "c") return 1;
	else if(str == "P" || str == "p") return 2;
	else return -1;
}
/*Secilen sutun hamle yapmak icin uygun mu?*/
int ConnectFour::isLegal(char coordinate)const{

    int i=0,j=0;
    
    j = coordinate - 'a';/*sutun hesaplama*/
	/*sutun degeri hatali girilmis ise*/
    if(j<0 || j >= currentWidth()){
        return -1;
    }
	/*asagidan yukari dogru , o sutunde ilk bulunan bos yer return edilir*/
    for(i=currentHeight()-1 ; i>=0 ; --i)
        if(gameCells[i][j].getValue() == '.')
            return i;
    return -1;/*sutun dolu ise -1 return edilir*/
}

/*yapilan hamle ile oyun kazanildi mi? Tum kontrolleri calistirir.
  Parametredeki  harf  'X' veya 'O' icin mi bakilacak,
  parametredeki  test 0 ise kazanilan oyunda harfleri kucuk harf yapar,
				 test 1 ise yapay zeka hamle test ediyordur.
				 oynanmamis hamle oldugundan harfleri kucultmez*/
int ConnectFour::controls(char harf,bool test){
    
    int kazanan=0 , sonuc=0;
   
    sonuc = (yatayKontrol(harf,test)    ||
             dikeyKontrol(harf,test)    ||
             sagCaprazKontrol(harf,test)||
             solCaprazKontrol(harf,test)
            );
    if(sonuc){
	
        if(harf == 'X')
            kazanan=1;
        else
            kazanan=2;
        if(test==0)
        	setGameEnd();
	}
    return kazanan;
}
/*yanyana 4 tane 'O' veya 'X' arar.*/
int ConnectFour::yatayKontrol(char harf,bool test){

    int i=0,j=0,find=0;
    
    for(i=0;i<currentHeight(); ++i)
        for(j=0;j+3<currentWidth(); ++j)
            if( gameCells[i][j].getValue() == harf &&
                gameCells[i][j+1].getValue() == harf &&
                gameCells[i][j+2].getValue() == harf &&
                gameCells[i][j+3].getValue() == harf    
              ){
                if(test==0){ /*kazanma sonrasi harfler kucultulur*/
                    gameCells[i][j].set(harf-('Z'-'z'));
                    gameCells[i][j+1].set(harf-('Z'-'z'));
                    gameCells[i][j+2].set(harf-('Z'-'z'));
                    gameCells[i][j+3].set(harf-('Z'-'z'));
                }
                find = 1;
            }
    return find;
}
/*dikey olarak 4 tane 'X' veya 'O' arar*/
int ConnectFour::dikeyKontrol(char harf,bool test){
    
    int i=0,j=0,find=0;
    
    for(j=0;j<currentWidth();++j)
        for(i=0;i+3<currentHeight();++i)
            if( gameCells[i][j].getValue() == harf &&
                gameCells[i+1][j].getValue() == harf &&
                gameCells[i+2][j].getValue() == harf &&
                gameCells[i+3][j].getValue() == harf    
              ){
                if(test==0){
                    gameCells[i][j].set(harf-('Z'-'z'));
                    gameCells[i+1][j].set(harf-('Z'-'z'));
                    gameCells[i+2][j].set(harf-('Z'-'z'));
                    gameCells[i+3][j].set(harf-('Z'-'z'));
                }
                find =1;
            }
    return find;
}
/*Sag asagi capraz olarak 4 tane 'X' veya 'O' arar*/
int ConnectFour::sagCaprazKontrol(char harf,bool test){
    int i=0,j=0,find=0;
    
    for(i=0;i+3<currentHeight();++i)
        for(j=0;j+3<currentWidth();++j)
            if( gameCells[i][j].getValue() == harf &&
                gameCells[i+1][j+1].getValue() == harf &&
                gameCells[i+2][j+2].getValue() == harf &&
                gameCells[i+3][j+3].getValue() == harf    
              ){
                if(test==0){
                    gameCells[i][j].set(harf-('Z'-'z'));
                    gameCells[i+1][j+1].set(harf-('Z'-'z'));
                    gameCells[i+2][j+2].set(harf-('Z'-'z'));
                    gameCells[i+3][j+3].set(harf-('Z'-'z'));
                }
                find = 1;
            }
    return find;
}
/*Sol asagi capraz olarak 4 tane 'X' veya 'O' arar*/
int ConnectFour::solCaprazKontrol(char harf,bool test){
    int i=0,j=0,find=0;
    
    for(i=currentHeight()-4;i>=0;--i)
        for(j=currentWidth()-1;j>=0;--j)
            if( gameCells[i][j].getValue() == harf &&
                gameCells[i+1][j-1].getValue() == harf &&
                gameCells[i+2][j-2].getValue() == harf &&
                gameCells[i+3][j-3].getValue() == harf    
              ){
                if(test==0){
                    gameCells[i][j].set(harf-('Z'-'z'));
                    gameCells[i+1][j-1].set(harf-('Z'-'z'));
                    gameCells[i+2][j-2].set(harf-('Z'-'z'));
                    gameCells[i+3][j-3].set(harf-('Z'-'z'));
                }
                find = 1;
            }
    return find;
}
/*Board daha sade goruntulensin diye ekranda satir atlar*/
void clearScreen(){
    int i=0;
    
    for(i;i<30;++i)
        cout << endl;

}

/*Board'u ekrana yazdirma*/
void ConnectFour::printBoard()const{

    int i=0;
	int j=0;
    for (i=0; i < currentWidth() ; ++i)
        cout << static_cast<char>('a'+i) << " ";
    cout << endl;
    for(i=0 ; i< currentHeight(); ++i){
        for(j=0; j < currentWidth(); ++j)
            cout << gameCells[i][j].getValue()<<" ";
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
char ConnectFour::play(){
    
    int i=0,row,col;
    char hamle='a',hamle2;
	/*hamle2 ,  2 hamle sonrasini hesaplamak icin*/    

	/*CPU oyunu tek hamlede bitirebilir mi?*/
    hamle = oyunBiterMi('O');
    if(hamle != '0')
        return hamle;

    /*Oyuncu oyunu tek hamlede bitirebilir mi?*/
    hamle = 'a';
    hamle = oyunBiterMi('X');
    if(hamle != '0')
        return hamle;
    
    /*2 hamle İlerisi için atak hesaplaması*/
	hamle = 'a';
	for(i=0 ; i<currentWidth(); ++i){
		row=isLegal(hamle);
		if(row!=-1){
		col=hamle - 'a';
		gameCells[row][col].set('O');/*Eger buraya hamle yaparsa*/
		hamle2 = oyunBiterMi('O');/*burada oyun tek hamlelik mi olur?*/
		gameCells[row][col].set('.'); /*Oyle ise hamle'yi return eder*/
		if(hamle2 != '0')
			return hamle;
		}
	++hamle;  
	}
  
	/*2 hamle sonrasinda oyun kazanilabilecek durumda
	  degil ise random bir hamle yapar*/
    srand(time(NULL));
   
    do{
        col = rand()% currentWidth();
        hamle = 'a';
        for (i=0 ;i<col; ++i)
            ++hamle;        /*karakter artirma*/
        if(isLegal(hamle)!=-1)
            return hamle;
    }while(1);/*return edene kadar hamle arar*/
}
/*Yapay zekanin yapacagi bir hamle ile oyunu bitirebilip , bitiremeyeceginin
     testidir.Yapay zeka tarafindan defalarca calistirilir ve uygun hamle 
     tespit edilir.
  Harf olarak 'O' gonderilir ise , yapay zeka kendisinin oyunu bitirebilip 
     bitiremeyecegine bakar. Harf olarak 'X' gonderilir ise , yapay zeka 
     oyunu rakibinin bitirebilip bitiremeyecegine bakar. 
  Eger oyun tek hamlede bitirilebilir duruma gelmemis ise '0' return edilir*/
char ConnectFour::oyunBiterMi(char harf){

    int i=0,j=0,row,col,bitis=1;/*bitis 1  CPU , bitis 2  user icin kontrol*/
    
    char hamle='a';
   
    if (harf=='O')/*harf 'X' ise bitis=1 , harf 'O' ise bitis=2*/
        bitis = 2;
    
    for(i=0; i<currentWidth() ;++i){
        row=isLegal(hamle);
        if(row!=-1){
            col=hamle - 'a';
            gameCells[row][col].set(harf);
            if(controls(harf,1)==bitis){
                gameCells[row][col].set('.');
                return hamle;
            }
            else
                gameCells[row][col].set('.');
        }
        ++hamle;
    }
    return '0';
}
void ConnectFour::saveGame(string str)const{

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
		fout << currentHeight()<<endl;
		fout << currentWidth()<< endl;
		fout << getOyunModu()<< endl;
		fout << getPlayer() << endl;
		for (i=0 ; i<currentHeight(); ++i){
			for(j=0; j<currentWidth() ; ++j)
				fout << gameCells[i][j].getValue();
			fout << endl;
		}
	
		fout.close();/*dosya kapama*/
		clearScreen();
		cout << "Save islemi basari ile tamamlandi."<<endl;
		printBoard();
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
void ConnectFour::loadGame(string str){

	int i=0,j=0,readValue;
	bool readP;
	string input;	
	
	/*.txt mi?*/
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
			setHamleSayisi(0);
		finp >> readValue ; /*reading height*/
			yukseklik = readValue;
		finp >> readValue ; /*reading width*/
			genislik = readValue;
		finp >> readValue ;/*reading oyunModu*/
			setOyunModu(readValue);
		finp >> readP ;/*reading player*/
			setPlayer(readP);
		
	
		for(i=0;i<yukseklik;++i)
			delete[] gameCells[i];
		delete[] gameCells;
		
		/*yer alma islemleri*/
		gameCells = new Cell*[yukseklik];
		for(i=0; i<yukseklik ;++i)
			gameCells[i]= new Cell[genislik];

		/*gameCell bos hucreler ile doldurulur
		  bir sonraki asamada ise , okunacak dosyada 
	      '*' karakteri gordugunde yerine '.' yazilacaktir*/
		for(i=0;i<yukseklik;++i){
			for(j=0;j<genislik;++j)
				gameCells[i][j] = Cell(i,j,' ');
		}

		getline(finp,input);/*ilk satirda bos satir okuma hatasi icin*/
		for (i=0 ; i<yukseklik; ++i){
			getline(finp,input);/*dosyadan satir satir okunur*/
			for(j=0; j<genislik ; ++j){
				for(int k=0; k<input.size();++k)
				/*okunan satir stringi harf harf yazilir*/
					gameCells[i][k].set(input[k]);
				if(gameCells[i][j].getValue() != '.' && 
				   gameCells[i][j].getValue() != ' ')
					hamleSayisi +=1;/*hamleler sayilir*/
			}
		}
	
		finp.close(); /*dosya kapama*/
		clearScreen();
		
		/*success durumunda  oyun bilgileri print edilir*/
		cout <<"Load islemi basari ile tamamlandi."<<endl
			 <<"Oyun Bilgileri : "<< endl;
		  	 if(getOyunModu()==1)
		  		 cout << "Oyun Modu : Player vs CPU  - Sira Player'da. " << endl;
			 else{
				 cout << "Oyun Modu: Player1 vs Player2" << endl;
				 if(getPlayer())
					cout <<"Sira Player 1'de."<< endl;
			     else
				 	cout <<"Sira Player 2'de."<< endl;
			 }
		printBoard();
	}
	else /*Hatali filename*/
		cerr << "**ERROR : Hatali filename." << endl;
}
