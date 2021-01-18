#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

//data structure representing a city with a pointer to another city
struct miasto {
	int symbol;
	miasto* kolejne;
};

struct check {
	int pierwsze;
	int drugie;
};

//get map dimensions from the user
void genMap(int& h, int& w, int& cities)
{
	cout << "Podaj wysokosc mapy: ";
	cin >> h;
	cout << endl << "Podaj szerokosc mapy: ";
	cin >> w;
	cities = h * w;
}

//initialize data structures
void initStruct(miasto**& graf, miasto**& drzewo, bool*& odwiedzone, int cities)		//inicjalizacja pustych struktur danych 
{
	for (int i = 0; i < cities; i++)
	{
		graf[i] = NULL;
		drzewo[i] = NULL;
		odwiedzone[i] = false;
	}
}

//add new road to the map
void dodajMiasto(miasto*& beg, int end)
{
	miasto* wsk = new miasto;
	wsk->symbol = end;
	wsk->kolejne = beg;
	beg = wsk;
}

//check whether a connection between two cities is already in the map
bool juzbylo(check komparator[], int m1, int m2, int i)     //funkcja sprawdzająca czy generator nie podał pary miast która już wcześniej została dodana 
{
	for (int j = 0; j <= i; j++)
	{
		if (((komparator[j].pierwsze == m1) and (komparator[j].drugie == m2)) or ((komparator[j].pierwsze == m2) and (komparator[j].drugie == m1)))
			return true;
	}
	return false;
}

//create random roads between cities and append them to the map 
void dodajDrogi(int cities, miasto**& graf, int width, int height)		//funkcja odpowiedzialna za generowanie losowych par miast oraz dopisywanie ich do mapy
{
	int miasto1 = 0;
	int miasto2 = 0;
	int zakres = (height * (width - 1) + width * (height - 1)) - (cities );
	int drogi = (rand() % zakres) + cities - (height);
	
	check* komparator = new check[drogi];
	komparator[0].pierwsze = 0;
	komparator[0].drugie = 0;

	for (int i = 0; i < drogi; i++) {
		miasto1 = (rand() % cities) + 1;
		miasto2 = (rand() % cities) + 1;
		
		int bezw = abs(miasto1 - miasto2);
		
		//check if road between random cities exists
		//		 exists - try again
		//       doesn't exist - append the map
		if (((bezw != 1) and (bezw != width)) or ((miasto1 % width == 0) and (miasto2 % width == 1)) or ((miasto2 % width == 0) and (miasto1 % width == 1)) or (miasto1 == miasto2) or juzbylo(komparator, miasto1, miasto2, i))		//zamiast warunku czy moga byc para zrobilem czy nie moga byc para
		{
			--i;
		}
		else
		{
			dodajMiasto(graf[miasto1 - 1], miasto2);
			dodajMiasto(graf[miasto2 - 1], miasto1);

			komparator[i].pierwsze = miasto1;
			komparator[i].drugie = miasto2;
		}
	}
	delete[] komparator;
}

//set of boolean functions used in map printing
//functions check where roads and water pipes exist
bool sprawdzDrogePoziomo(int i, miasto** graf)
{
	miasto* p;
	int symbol;
	p = graf[i - 1];
	while (p)
	{
		symbol = p->symbol;
		if (symbol == i + 1)
		{
			return true;
		}
		else p = p->kolejne;
	}
	return false;
}

bool sprawdzDrogePionowo(int j, miasto** graf, int width)
{
	miasto* p;
	int symbol;
	p = graf[j];
	while (p)
	{
		symbol = p->symbol;
		if (symbol == j + 1 + width)
		{
			return true;
		}
		else p = p->kolejne;
	}
	return false;
}

bool sprawdzWodPoziomo(int i, miasto** drzewo)
{
	miasto* p;
	miasto* r;
	int symbol;
	p = drzewo[i - 1];
	r = drzewo[i];
	while (p)
	{
		symbol = p->symbol;
		if (symbol == i + 1)
		{
			return true;
		}
		else
		{
			p = p->kolejne;
		}
	}
	while (r)
	{
		symbol = r->symbol;
		if (symbol == i)
		{
			return true;
		}
		else
		{
			r = r->kolejne;
		}
	}
	return false;
}

bool sprawdzWodPionowo(int j, miasto** drzewo, int width)
{
	miasto* p;
	miasto* r;
	int symbol;
	p = drzewo[j];
	r = drzewo[j + width];
	while (p)
	{
		symbol = p->symbol;
		if (symbol == j + 1 + width)
		{
			return true;
		}
		else p = p->kolejne;
	}
	while (r)
	{
		symbol = r->symbol;
		if (symbol == j + 1)
		{
			return true;
		}
		else r = r->kolejne;
	}
	return false;
}

//print vertical connections
void piszPion(int i, int width, miasto** graf, int w, miasto** drzewo)
{
	int j = i - width;
	while ((j + 1) % width != 0)
	{
		if (w and sprawdzWodPionowo(j, drzewo, width))
		{
			cout << "||    ";
		}
		else if (sprawdzDrogePionowo(j, graf, width))
		{
			cout << " |    ";
		}
		else cout << "      ";
		j++;
	}
	if (w and sprawdzWodPionowo(j, drzewo, width))
	{
		cout << "||" << endl;
	}
	else if (sprawdzDrogePionowo(j, graf, width))
	{
		cout << " |" << endl;
	}
	else cout << endl;
}


//print indexes of the cities and horizontal connections
void piszMape(int cities, int width, miasto** graf, miasto** drzewo)
{
	for (int w = 0; w < 2; w++)
	{
		if (!w)
		{
			cout << endl;
			for (int k = 0; k < (6 * width) - 4; k++)
				cout << "*";
			cout << "\nMapa Drog:" << endl;
			for (int k = 0; k < (6 * width) - 4; k++)
				cout << "*";
			cout << endl;
		}
		for (int i = 1; i <= cities; i++)
		{
			if (i % width != 0)
			{
				cout << setw(2) << i;
				if (w and sprawdzWodPoziomo(i, drzewo))		//wszytskie funkcje typu sprawdz... sprawdzaja istnienie danego polaczenia w grafie badz drzewie
				{
					cout << "====";
				}
				else if (sprawdzDrogePoziomo(i, graf))
				{
					cout << "----";
				}
				else cout << "    ";
			}
			else if (i != cities)
			{
				cout << setw(2) << i << endl;
				piszPion(i, width, graf, w, drzewo);			//wywolanie funkcji wypisujacej pionowe drogi na koncu kazdej linijki oprocz ostatniej
			}
			else
			{
				cout << setw(2) << i;
			}
		}
		if (!w)
		{
			cout << endl;
			for (int k = 0; k < (6 * width) - 4; k++)
				cout << "*";
			cout << "\nMapa Wodociagow:" << endl;
			for (int k = 0; k < (6 * width) - 4; k++)
				cout << "*";
			cout << endl;
		}
	}
}

//recurrent DFS algorithm
void DFS(int s, bool*& odwiedzone, miasto** graf, miasto**& drzewo)
{
	miasto* p, * r;
	int indekssasiada;
	odwiedzone[s] = true;

	for (p = graf[s]; p; p = p->kolejne)
	{
		indekssasiada = (p->symbol) - 1;

		if (!odwiedzone[indekssasiada]) {
			r = new miasto;
			r->symbol = indekssasiada + 1;
			r->kolejne = drzewo[s];
			drzewo[s] = r;
			DFS(indekssasiada, odwiedzone, graf, drzewo);
		}
	}
}

//check if the algorithm visited all cities
//if not create new roads and run algorithm again
void sprawdzDrzewo(int cities, bool*& odwiedzone, int width, miasto**& graf, miasto**& drzewo, int height, int pierwszyWierzcholek)
{
	for (int i = 0; i < cities; i++) {
		if (odwiedzone[i] == false) {
			initStruct(graf, drzewo, odwiedzone, cities);
			dodajDrogi(cities, graf, width, height);
			DFS(pierwszyWierzcholek, odwiedzone, graf, drzewo);
			sprawdzDrzewo(cities, odwiedzone, width, graf, drzewo, height, pierwszyWierzcholek);
		}
	}
}

//clean the memory
void kasujpamiec(miasto**& graf, miasto**& drzewo, int cities, bool*& odwiedzone)
{
	miasto* m;
	miasto* n;
	for (int i = 0; i < cities; i++)
	{
		m = graf[i];
		while (m)
		{
			n = m;
			m = m->kolejne;
			delete n;
		}
		m = drzewo[i];
		while (m)
		{
			n = m;
			m = m->kolejne;
			delete n;
		}
	}
	delete[] graf;
	delete[] drzewo;
	delete[] odwiedzone;
}


int main()
{
	srand(time(NULL));
	int height;
	int width;
	int cities;
	int pierwszyWierzcholek = 0;

	genMap(height, width, cities);     //take the inputs from the user

	miasto** graf = new miasto * [cities];		//struktura danych opisujaca polaczenia drog - dynamic matrix for road connections between cities
	miasto** drzewo = new miasto * [cities];	//struktura danych opisujaca polaczenia wodociagow - dynamic matrix for water canals between cities
	bool* odwiedzone = new bool[cities];		//pomocnicza tablica odwiedzonych wierzcholkow - dynamic vector for checking which cities were visited

	initStruct(graf, drzewo, odwiedzone, cities);
	
	dodajDrogi(cities, graf, width, height);    //create roads
	
	DFS(pierwszyWierzcholek, odwiedzone, graf, drzewo);   //run the algorithm on the map with roads
	
	sprawdzDrzewo(cities, odwiedzone, width, graf, drzewo, height, pierwszyWierzcholek);   //check if all cities are connected together
	
	piszMape(cities, width, graf, drzewo);    //print the map
	
	kasujpamiec(graf, drzewo, cities, odwiedzone);   //clean the memory
}
