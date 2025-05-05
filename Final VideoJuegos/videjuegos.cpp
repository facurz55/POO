#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>
using namespace std;


class Producto{
protected:
	int id;
	char nombre[100];
	char descripcion[200];
	double costo;
public:
	Producto(const char* nom, const char* descrip, int costo_, int id_){
		strcpy(nombre,nom);
		strcpy(descripcion, descrip);
		this->costo = costo_;
		this->id = id_;
	}
	const char* getNombre(){return nombre;}
	const char* getDescrip(){return descripcion;}
	double getCosto(){return costo;}
	int getId(){return id;}
};


class Parche: public Producto{
private:
	vector<Producto*>depedencias;
public:
	Parche(const char* nom, const char* descrip, int costo_, int id_): Producto(nom,descrip,costo_,id_){}
	void agregarDependencias(Producto* nueva){
		depedencias.push_back(nueva);
	}
	vector<Producto*> getDependencias(){
		return depedencias;
	}
};

class Juego: public Producto{
private:
	vector<Parche*>parches;
public:
	Juego(const char* nom, const char* descrip, int costo_, int id_): Producto(nom,descrip,costo_, id_){}
	void agregarParches(Parche* nuevo){
		parchesInstalados.push_back(nuevo);
	}
	vector<Parche*>getParches(){
		return parches;
	}
	
};

class Gestor{
private:
	vector<Producto*>productos;
public:
	///CONSIGNA 1
	void AgregarProducto(Producto* nuevo){
		productos.push_back(nuevo);
	}
	
	///CONSIGNA 2
	void juegosYParchesBinario(Producto* nuevo, string dirArch){
		ofstream archivo(dirArch, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int id = nuevo->getId();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		
		int tam = strlen(nuevo->getNombre()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam),sizeof(int));
		archivo.write(nuevo->getNombre(), tam);
		
		tam = strlen(nuevo->getDescrip()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam),sizeof(int));
		archivo.write(nuevo->getDescrip(),tam);
		
		double costo = nuevo->getCosto();
		archivo.write(reinterpret_cast<const char*>(&costo),sizeof(double));
		
		archivo.close();
	}	
	
	void relacionesBinario(Producto* p, string dirArch){
		ofstream archivo(dirArch, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		Juego* jueguito = nullptr;
		
		if(dynamic_cast<Juego*>(p)){
			jueguito = dynamic_cast<Juego*>(p);
			
			int id = jueguito->getId();
			archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
			
			int cantidad = jueguito->getParches().size();
			archivo.write(reinterpret_cast<const char*>(&cantidad),sizeof(int));
			
			for(const auto& c : jueguito->getParches()){
				id = c->getId();
				archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
			}
		}
		
		archivo.close();
	}

	void dependenciasParchesBinario(string dirArch, Producto* parche_){
		ofstream archivo(dirArch, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		Parche* parche = dynamic_cast<Parche*>(parche_);
		
		int id = parche->getId();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		
		int cantidad = parche->getDependencias().size();
		archivo.write(reinterpret_cast<const char*>(&cantidad), sizeof(int));
		
		for(const auto p: parche->getDependencias()){
			id = p->getId();
			archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		}
		
		archivo.close();
	}

	void guardarBinario(string dirJuego, string dirParche, string dirRelacion, string dirDepenParche){
		if(productos.empty()){return;}
		
		for(const auto& p: productos){
			if(dynamic_cast<Juego*>(p)){
				relacionesBinario(p,dirRelacion);
				juegosYParchesBinario(p,dirJuego);
			}else if(dynamic_cast<Parche*>(p)){
				dependenciasParchesBinario(p,dirDepenParche);
				juegosYParchesBinario(p,dirJuego);
			}
		}
		///RECOMENDACION SI NO QUERES TENER CUATRO DIRECCIONES DE ARCHIVOS (PORQUE ES MUY PAJA).
		///AGREGALE A LA CLASE BASE UN CHAR TIPO, PARA A LA HORA DE LECTURA PUEDAS IDENTIFICAR QUE 
		///TIPO DE PRODUCTO ES Y SOLAMENTE TENES TRES DIRECCIONES DE ARCHIVOS EN LUGAR DE 4.
		/*DE TODAS FORMAS ESTA OPCION TAMBIEN ES FUNCIONAL*/
	}
	
	
	///CONSIGNA 3
	vector<Juego*> juegosMasCaros(){
		if(productos.empty()){return;}
		
		vector<Juego*>juegos;
		
		for(const auto& p: productos){
			if(dynamic_cast<Juego*>(p)){
				Juego* jueguito = dynamic_cast<Juego*>(p)
				juegos.push_back(jueguito);
			}
		}
		
		sort(juegos.begin(),juegos.end(), [](const auto& a, const auto&b){return a->getCosto()>b->getCosto();});
		double costoMax = juegos[0]->getCosto();
		int contador = 0;
		
		
		for(const auto& p: juegos){
			if(costoMax == p->getCosto()){
				cout<<p->getNombre()<<endl;
				contador++;
			}
		}
		
		juegos.resize(contador);
		return juegos;
	}
	
	vector<Juego*> juegoConMasParches(){
		if(productos.empty()){return;}
		
		vector<juego*>juegos;
		
		for(const auto& p: productos){
			if(dynamic_cast<Juego*>(p)){
				Juego* jueguito = dynamic_cast<Juego*>(p)
					juegos.push_back(jueguito);
			}
		}
		
		sort(juegos.begin(),juegos.end(), [](const auto& a, const auto&b){return a->getParches().size() > b->getParches().size();});
		int parchesMax = juegos[0]->getCosto();
		int contador = 0;
		
		for(const auto& p: juegos){
			if(parchesMax == p->getCosto()){
				cout<<p->getNombre()<<endl;
				contador++;
			}
		}
		
		juegos.resize(contador);
		return juegos;
	}
	
	void crearTXT(string dirArch){
		if(productos.empty()){return;}
		
		ofstream archivo(dirArch);
		
		if(archivo.fail()){return;}
		
		vector<Juego*>mayorCosto;
		vector<Juego*>masPaches;
		
		mayorCosto = juegosMasCaros();
		masParches = juegoConMasParches();
		
		archivo<<"Juegos con mayor costo: "<<endl;
		for(const auto& p: mayorCosto){
			archivo<<p->getNombre()<<" Costo :"<<p->getCosto()<<endl;
		}
		
		archivo<<endl<<endl;
		
		archivo<<"Juegos con mas Parches: "<<endl;
		for(const auto& p: masParches){
			archivo<<p->getNombre()<<" Numero de parches: "<<p->getParches().size()<<endl;
		}
		
		archivo.close();
	}
	
};

///CONSIGNA 4
/*
¿Qué ventajas y desventajas tiene la recursividad en C++?
Ventajas:

-Permite resolver problemas complejos de forma clara y reducida.

-Reduce la cantidad de código en problemas que tienen una estructura repetitiva o jerárquica (como árboles o algoritmos divide y vencerás).

Desventajas:

-Puede consumir mucha memoria por el uso de la pila.

-Es menos eficiente que la iteración en muchos casos.

-Puede causar errores como desbordamiento de pila si no se controla bien la condición de corte.


¿Por qué C++ necesita de herencia para implementar el polimorfismo?
C++ necesita herencia porque el polimorfismo dinámico se basa en que diferentes clases compartan una misma interfaz (clase base). 
Usando punteros o referencias a esa clase base, se pueden llamar métodos que se comportan distinto según el objeto real 
(esto se logra con funciones virtuales).



*/


int main(int argc, char *argv[]) {
	
	return 0;
}

