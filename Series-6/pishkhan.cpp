#include <iostream>
#include <string>
#include <map>
#include <vector>
// be nazar miyad kolan niyazi be timestamp nadarim 
// chon asan niyazi be khroji dadaneshon nist va hame chiz be tatib zamani ham dade mishan 
using namespace std ; 
struct src {
	int second ; 
	int msecond ; 
	string strings[100] ; 
	int ints[10] ;
	int l ; 
};
class Restaurant{
	private:
		int n , m , k ; 
		// n is the number of evidence 
		// m is the number of foods 
		// k is the table 
		struct src t ;
		int A[100] ; 
		map<string , int> foods;
		string buf1 ;
		string buf2 ; 
		char bufc ; 
		int bufn ; 
		//map<int, vector<int>> table ;// 2D array/ [0] : capacity , [1] : money , [2] : payment_num , [3] : second , [4] : msecond
		int table[200][4] = {} ;
		// from 1-100 is the table and the 100-200 is waiting list 
		int money  = 0; 
		int finalmoney = 0 ; 
		int payment_num = 1 ;
	public :
		string s ; 
		Restaurant(){
			scanf("%d %d %d",&n,&m,&k);
			// I make the menu here ...
			for(int i = 0 ; i < m ; ++i){
				cin >> buf1 >> buf2 ; 
				bufn = stoi(buf2);	
				foods[buf1] = bufn ;
			}
			for(int i = 0 ; i < k ; ++i){
				cin >> table[i][0] ; 
				/*
				   table[i][1] = 0 ; 
				   table[i][2] = 0 ; 
				   table[i][3] = 0 ; 
				   table[i][4] = 0 ; 
				   */
			}
			cin >> bufc ; 
			cout << bufc ;
		}

		void start(){
			for(int i = 0 ; i < n ; ++i ){
				getline(cin, s) ;

				int j = 0 ; 
				for(int i = 0 ; i <= s.length() ; i++ ) {
					if((int) s[i] == 32 ){
						A[j] = i ; 
						j++ ; 
					}
				}
				A[j] = s.length() ;

				// So it's better to do the preprocessing on the input text before pass it to any function
				// We khow that the input which start with number are always int and in other case they are strings
				// for this problem I can use "int num = stoi(str)" to cast between the string and integer 

				string buf = "" ; 

				int k = 0 , l = 0 ; 
				for(int i = 0 ; i < j ; i++ ){
					buf = ""; 
					buf = s.substr(A[i]+1 ,(A[i+1])-(A[i]+1)) ;
					// Pay attention that the second argument here is the length of the substr not the endpoint ...
					// and also the length get the initialize point as well ...
					t.strings[l] = buf ; 
					l++; 

					// ---I Love HASHING---
					//I can use the first character and the length which I have No collision at all .
					//cout << l ; 
				}
				t.l = l ; 
				//parsing the timestamps  
				string buffer = "" ; 
				int num ; 
				int B[] = {-1,2,5,9} ;
				for(int i = 0 ; i < 3 ; i++){

					//cout << l ;
					//cout << t.strings[l-1] ; 
					buffer = t.strings[l-1].substr(B[i]+1, (B[i+1] - B[i]-1));
					//cout << buffer << "\n";
					num = stoi(buffer);
					//cout << num << "\n" ;

					if(i == 0 ){
						// We initialize the last_second for each ten second update in money
						t.second = num*60 ;
					}

					if(i == 1){
						t.second  += num ; 
					}

					if(i == 2){
						t.msecond = num  ;
					}

				}
				// another parser for strings to ints 
				//

				int hash_score = (int) s[0];
				if(hash_score == 111 ){
					if(s.length() == 5) {
						hash_score = 1 ;  
					}else{
						hash_score = 2 ; 
					}
				}

				switch(hash_score){
					case 1:
						order(&t);  
						break;		
					case 2:
						order_status(&t);
						break;
					case 112:
						payment(&t);
						break;
					case 116:  
						table_status(&t);
						break;
					case 103:
						general_status(&t);
						break;
				}
				for(int i = 0 ; i <= j ; i++){
					A[i] = 0 ;
				}

			}
		}
		void order(struct src *t ){
			int l = t->l ; 
			int flag = 1;
			int bufn = stoi(t->strings[l-1]);
			int table_index = 101 ;
			int table_num = 16 ;// the maximu is 15
			int moneybuf = 0 ;
			//
			for(int i = 0 ; i <= 100 ; i++){
				if(table[i][0] >= bufn && table[i][0] < table_num){
					if(table[i][2] == 0 ){
						// this mean that the table is free 
						table_index = i ; 
						table_num = table[i][0] ; 	
						flag = 0 ; 	
					}else{
						if(flag){
							// this mean that the table is free 
							table_index = i ; 
							table_num = table[i][0] ; 
						}	
					}
				}
			}
			// i == k
			if(table_index == 101 )
			{
				cout << "not enough seat." << "\n" ; 
				// it mean there is no table with that amount 
			}else{
				for(int i = 0 ; i < (t->l-2) ; i++){
					bufc = t->strings[l+1][t->strings[l+1].length() - 1];
					buf1 = "" ;
					buf1 = buf1 + bufc ; 
					bufn = stoi(buf1) ; 
					moneybuf += foods[t->strings[l+1]]*bufn ;
				}

				if(flag){
					// this mean there is a table but it has been occupied 
					for(int i = 101 ; i<= 200 ; i++){
						if(table[i][2] == 0 ){
							money += moneybuf ; 
							table[table_index][1] += moneybuf ; 
							table[table_index][2] += payment_num ; 
							payment_num++ ;
							break ;
						}
					}
				cout << "please wait for free table." << "\n" ; 
			}else{
				money += moneybuf ; 
				table[table_index][1] += moneybuf ; 
				table[table_index][2] += payment_num ; 
				table[table_index][3] += t->second ; 
				table[table_index][4] += t->msecond ; 
				payment_num++ ; 
				cout << "please sit at table number " << table_index << "." << "\n"; 
			}
		}
		}

		void payment(struct src *t){
			bufn = stoi(t->strings[1]);
			int n = 101; 
			for(int i = 0 ; i <= 200 ; i++){
				if(table[i][2] == bufn){
					n = i ; 
					break;
				}
			}
			if(n > 100 ){
				cout << "pays after eating." << "\n" ; 	
			}else{
				bufn = table[n][1] ; 
				for(int i = 101 ; i <= 200 ; i++){
					if(table[i][0] <= bufn){
						cout << "you should pay " << table[n][1] ; 
						table[n][1] += table[i][1] ; 
						table[n][2] += table[i][2]; 
						table[n][3] += t->second + 120 ; 
						table[n][4] += t->msecond ;
						table[i][1] = 0 ; 
						table[i][2] = 0 ;
						break ; 
					}
				}
			}
		}
		void order_status(struct src *t){}
		void table_status(struct src *t){}
		void general_status(struct  src *t){}
};

int main(){

	// there is also another way to parsing the input string and that is to use the find method ...
	Restaurant restaurant ;
	restaurant.start();
	//restaurant.

}