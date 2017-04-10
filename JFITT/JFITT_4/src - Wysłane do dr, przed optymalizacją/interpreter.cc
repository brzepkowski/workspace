/*
 * Kod interpretera maszyny rejestrowej do projektu z JFTT2015
 *
 * Autor: Maciek Gębala
 * http://mgc.im.pwr.wroc.pl/
 * 2015-11-14
*/

#include<iostream>
#include<fstream>

#include<tuple>
#include<vector>
#include<map>

#include<cstdlib> 	// rand()
#include<ctime>

using namespace std;

enum Instructions { READ, WRITE, LOAD, STORE, COPY, ADD, SUB, SHR, SHL, INC, DEC, RESET, JUMP, JZERO, JODD, HALT, ERROR };

int main(int argc, char* argv[])
{
    vector< tuple<Instructions,int,int> > program;
    map<int,long long> pam;

    long long r[10];
    int lr;

    int k=0;
    long long i;
    Instructions i1;
    int i2, i3;
    string com;

    if( argc!=2 )
    {
	cout << "Sposób użycia programu: interpreter kod" << endl;
	return -1;
    }

    cout << "Czytanie programu." << endl;
    ifstream plik( argv[1] );
    if( !plik )
    {
	cout << "Błąd: Nie można otworzyć pliku " << argv[1] << endl;
	return -1;
    }
    while( !plik.eof() )
    {
	plik >> com;
	i1 = ERROR;
        i2 = 0;
        i3 = 0;

	if( com=="READ"  ) { i1 = READ; plik >> i2; }
	if( com=="WRITE" ) { i1 = WRITE; plik >> i2; }

	if( com=="LOAD"  ) { i1 = LOAD; plik >> i2; plik >> i3; }
	if( com=="STORE" ) { i1 = STORE; plik >> i2; plik >> i3; }

	if( com=="COPY"  ) { i1 = COPY; plik >> i2; plik >> i3; }
	if( com=="ADD"   ) { i1 = ADD; plik >> i2; plik >> i3; }
	if( com=="SUB"   ) { i1 = SUB; plik >> i2; plik >> i3; }
	if( com=="SHR"   ) { i1 = SHR; plik >> i2; }
	if( com=="SHL"   ) { i1 = SHL; plik >> i2; }
	if( com=="INC"   ) { i1 = INC; plik >> i2; }
	if( com=="DEC"   ) { i1 = DEC; plik >> i2; }
	if( com=="RESET" ) { i1 = RESET; plik >> i2; }

        if(i2>9) { cout << "Błąd: zły adress w instrukcji w linii " << k << endl; return -1; }
        if(i3>9) { cout << "Błąd: zły adress w instrukcji w linii " << k << endl; return -1; }

	if( com=="JUMP"  ) { i1 = JUMP; plik >> i2; }
	if( com=="JZERO" ) { i1 = JZERO; plik >> i2;  plik >> i3; }
	if( com=="JODD"  ) { i1 = JODD; plik >> i2;  plik >> i3; }
	if( com=="HALT"  ) { i1 = HALT; }

	if(i1==ERROR) { cout << "Błąd: Nieznana instrukcja w linii " << k << "." << endl; return -1; }
        if(i2<0) { cout << "Błąd: Zły adress w instrukcji w linii " << k << endl; return -1; }
        if(i3<0) { cout << "Błąd: Zły adress w instrukcji w linii " << k << endl; return -1; }

	if( plik.good() )
	{
	    program.push_back( make_tuple(i1,i2,i3) );
	}
	k++;
    }
    plik.close();
    cout << "Skończono czytanie programu (linii: " << program.size() << ")." << endl;

    cout << "Uruchamianie programu." << endl;
    lr = 0;
    srand(time(NULL));
    for(int i = 0; i<10; i++ ) r[i] = rand();
    i = 0;
    while( get<0>(program[lr])!=HALT )	// HALT
    {
	switch( get<0>(program[lr]) )
	{
	    case READ:	cout << "? "; cin >> r[get<1>(program[lr])]; i+=100; lr++; break;
	    case WRITE:	cout << "> " << r[get<1>(program[lr])] << endl; i+=100; lr++; break;

	    case LOAD:	r[get<1>(program[lr])] = pam[r[get<2>(program[lr])]]; i+=20; lr++; break;
	    case STORE:	pam[r[get<2>(program[lr])]] = r[get<1>(program[lr])]; i+=20; lr++; break;

	    case COPY:	r[get<1>(program[lr])] = r[get<2>(program[lr])] ; i+=1; lr++; break;
	    case ADD:   r[get<1>(program[lr])] += r[get<2>(program[lr])] ; i+=5; lr++; break;
	    case SUB:   if( r[get<1>(program[lr])] >= r[get<2>(program[lr])] )
                          r[get<1>(program[lr])] -= r[get<2>(program[lr])];
                        else
                          r[get<1>(program[lr])] = 0;
                        i+=5; lr++; break;
	    case SHR:   r[get<1>(program[lr])] >>= 1; i+=1; lr++; break;
	    case SHL:   r[get<1>(program[lr])] <<= 1; i+=1; lr++; break;
	    case INC:   r[get<1>(program[lr])]++ ; i+=1; lr++; break;
	    case DEC:   if( r[get<1>(program[lr])]>0 ) r[get<1>(program[lr])]--; i+=1; lr++; break;
	    case RESET: r[get<1>(program[lr])] = 0; i+=1; lr++; break;

	    case JUMP: 	lr = get<1>(program[lr]); i+=1; break;
	    case JZERO:	if( r[get<1>(program[lr])]==0 ) lr = get<2>(program[lr]); else lr++; i+=1; break;
	    case JODD:	if( r[get<1>(program[lr])] % 2 != 0 ) lr = get<2>(program[lr]); else lr++; i+=1; break;
		default: break;
	}
	if( lr<0 || lr>=(int)program.size() )
	{
	    cout << "Błąd: Wywołanie nieistniejącej instrukcji nr " << lr << "." << endl;
	    return -1;
	}
    }
    cout << "Skończono program (czas: " << i << ")." << endl;

    return 0;
}
