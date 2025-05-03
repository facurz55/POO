#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
using namespace std;

class Paquete{
private:
	char codigo[200];
	char nombre[200];
	int version;
	vector<Paquete*> dependencias;
public:
	Paquete(const char* cod, const char* nom, int version_){
		version = version_;
		strcpy(codigo,cod);
		strcpy(nombre, nom);
	}
	
	const char* getNombre(){
		return nombre;
	}
	
	const char* getCodigo(){
		return codigo;
	}
	
	int getVersion(){
		return version;
	}
	
	void agregarDependencia(Paquete* nuevo){
		dependencias.push_back(nuevo);
	}
	
	vector<Paquete*> getDependencias(){
		return dependencias;
	}

};


void escribirDependencias(ostream& os, Paquete* paquete, int nivel) {
	for (Paquete* dep : paquete->getDependencias()) {
		for (int i = 0; i < nivel; i++) os << '\t';
		os << dep->getNombre() << endl;
		escribirDependencias(os, dep, nivel + 1);
	}
}

ostream& operator<<(ostream& os, Paquete* paquete) {
	os << paquete->getNombre() << endl;
	escribirDependencias(os, paquete, 1);
	return os;
}

class Gestor{
private:
	vector<Paquete*> paquetes;
	
public:
	void cargarPaquetes(Paquete* paqueton){
		paquetes.push_back(paqueton);
	}
	///CONSIGNA 1)
	void leerArchivos(){
		ifstream dependencias("dependencias.dat", ios::binary);
		ifstream paquetes("paquetes.dat", ios::binary);
		
		if(paquetes.fail() || dependencias.fail()){
			return;
		}
		
		char codigo[200];
		char nombre[200];
		int version;
		char codPD[200];
		while(paquetes.read(codigo,sizeof(codigo))){
			paquetes.read(nombre,sizeof(nombre));
			paquetes.read(reinterpret_cast<char*>(&version), sizeof(version));
			
			Paquete* nuevo = new Paquete(codigo,nombre,version);
			this->paquetes.push_back(nuevo);
		}
		
		while(dependencias.read(codigo,sizeof(codigo))){
			dependencias.read(codPD,sizeof(codPD));
			
			for(const auto& p: this->paquetes){
				if(strcmp(p->getCodigo(),codigo) == 0){
					for(const auto& c: this->paquetes){
						if(strcmp(c->getCodigo(),codPD) == 0){
							p->agregarDependencia(c);
						}
					}
				}
			}
		}
		
		paquetes.close();
		dependencias.close();
	}
	
	///CONSIGNA 2)
	void crearArchivoDeTXT(Paquete* paquete) {
		string nombreArchivo = string(paquete->getCodigo()) + ".txt";
		ofstream archivo(nombreArchivo.c_str());
		if (!archivo.fail()) {
			archivo << paquete;
			archivo.close();
		}
	}
	
	///CONSIGNA 3)
	vector<Paquete*> top5MasDepedencias(){
		if(this->paquetes.empty()){
			return {};
		}
		
		vector<Paquete*>top5 = paquetes;
		
		sort(top5.begin(),top5.end(),[](const auto& a, const auto& b){return a->getDependencias().size()>b->getDependencias().size();});
		
		top5.resize(5);
		
		return top5;
	}
	
	vector<Paquete*> paquetesSinDependencias(){
		if(paquetes.empty()){return{};}
		
		vector<Paquete*>sinDependencias;
		
		copy_if(paquetes.begin(), paquetes.end(), back_inserter(sinDependencias),[](const auto& a){return a->getDependencias().empty();});
		
		return sinDependencias;
	}
};

///CONSIGNA 4
template <class T>
class vectorOrdenado{
private:
	T* datos;
	int capacidad;
	int cantidad;
	
	void redimensionar(){
		capacidad *= 2;
		T* nuevo = new T[capacidad];
		
		for(int i = 0; i< cantidad; i++){
			nuevo[i] = datos[i];
		}
		
		delete [] datos;
		datos = nuevo;
	}
	
	void ordenar_vector(){
		for (int i = 0; i<cantidad; i++){
			for(int j = 1; j<cantidad; j++){
				if(datos[i]<datos[j]){
					T aux = datos[i];
					datos[i] = datos[j];
					datos[j] = aux;
				}
			}
			
		}
	}
	
public:
	vectorOrdenado(): capacidad(2){
		cantidad = 0;
		datos = new T[capacidad];
	}
	
	void agregar(const T& elemento){
		if(cantidad == capacidad){
			redimensionar();
		}
		datos[cantidad] = elemento;
		cantidad ++;
		ordenar_vector();
	}
	
	T eliminarElemento(int indice){
		if(indice < 0 || indice >=cantidad){
			return {};
		}
		T pepe = datos[indice];
		for(indice; indice < cantidad -1; indice++){
			datos[indice] = datos[indice+1];
		}
		cantidad--;
		return pepe;
	}
	
	void imprimirElementos(){
		for(int i = 0; i < cantidad; i++){
			cout<<datos[i]<<endl;
		}
	}
};

int main(int argc, char *argv[]) {
	
	Gestor gestor;
	
	// Nivel 0 (raíz)
	Paquete* paqueteA = new Paquete("A", "Paquete A", 1);
	
	// Nivel 1
	Paquete* paqueteB = new Paquete("B", "Paquete B", 1);
	Paquete* paqueteC = new Paquete("C", "Paquete C", 1);
	Paquete* paqueteD = new Paquete("D", "Paquete D", 1);
	
	// Nivel 2
	Paquete* paqueteE = new Paquete("E", "Paquete E", 1);
	Paquete* paqueteF = new Paquete("F", "Paquete F", 1);
	Paquete* paqueteG = new Paquete("G", "Paquete G", 1);
	Paquete* paqueteH = new Paquete("H", "Paquete H", 1);
	
	// Nivel 3
	Paquete* paqueteI = new Paquete("I", "Paquete I", 1);
	Paquete* paqueteJ = new Paquete("J", "Paquete J", 1);
	Paquete* paqueteK = new Paquete("K", "Paquete K", 1);
	
	// Armar árbol de dependencias
	paqueteE->agregarDependencia(paqueteI);
	paqueteF->agregarDependencia(paqueteJ);
	paqueteG->agregarDependencia(paqueteJ);
	paqueteH->agregarDependencia(paqueteK);
	
	paqueteB->agregarDependencia(paqueteE);
	paqueteB->agregarDependencia(paqueteF);
	paqueteC->agregarDependencia(paqueteG);
	paqueteD->agregarDependencia(paqueteH);
	
	paqueteA->agregarDependencia(paqueteB);
	paqueteA->agregarDependencia(paqueteC);
	paqueteA->agregarDependencia(paqueteD);
	
	// Cargar paquete principal al gestor
	gestor.cargarPaquetes(paqueteA);
	
	// Crear archivo de salida
	gestor.crearArchivoDeTXT(paqueteA);
	
	cout << "Archivo generado: A.txt" << endl;
	
	return 0;
}

///CONSIGNA 4
/*
Se debe sobreescribir el operador < en el tipo T, ya que la estructura representa un vector dinámico ordenado.
Este operador permite comparar dos elementos del tipo T, lo cual es necesario para mantener el orden al insertar y al imprimir los datos.
*/


///CONSIGNA 5
/*
Es necesario utilizar composicion cuando debemos instanciar un objeto de una clase dentro otra clase, gracias a esto eliminamos redundancias y generamos un codigo mas optimo.

El polimorfismo si simplifica el codigo, basicamente consiste en adaptar metodos de la clase padre a las clases hijas, es decir, un metodo de la clase padre puede ser sobre escrito en la clase hija.
*/
