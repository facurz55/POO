#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;
class Dependencia{
private:
	char codigoPaquete[100];
	char codigoPaqueteDependencia[100];
public:
	const char* getCodDependencia(){
		return this->codigoPaqueteDependencia;
	}
};

class Paquete{
private:
	char codigo[100];
	char nombre[100];
	int version;
	vector<Dependencia*>dependencias;
public:
	vector<Dependencia*> getDependencias(){
		return dependencias;
	}
	
	const char* getCodigo(){
		return codigo;
	}
	
	const char* getNombre(){
		return nombre;
	}
	
	int cantDependencias(){
		int i = dependencias.size();
		return i;
	}
};

class Gestor{
private:
	vector<Paquete*>paquetes;
	void obtenerDependencia(const char* codigo, vector<char*>& visitados, vector<Dependencia*>&dependencias){
		for(const auto& v: visitados){
			if(strcmp(v,codigo)==0)return;
		}
		
		char* nuevo = new char[strlen(codigo)+1];
		strcpy(nuevo,codigo);
		visitados.push_back(nuevo);
		
		for (const auto& p : paquetes) {
			if (strcmp(p->getCodigo(), codigo) == 0) {
				for (const auto& d : p->getDependencias()) {
					dependencias.push_back(d);
					obtenerDependencia(d->getCodDependencia(), visitados, dependencias);
				}
			}
		}
		
	}
public:
	
	///CONSIGNA 1
	vector<Dependencia*> Dependencias(const char* cod){
		if(paquetes.empty()){return {};}
		
		vector<Dependencia*>depende;
		vector<char*> visitados;
		obtenerDependencia(cod, visitados, depende);
		
		return depende;
	}
	
	
	
	///CONSIGNA 2
	void archivoTexto(const char* cod){
		Paquete* paquetito = nullptr;
		if(paquetes.empty())return;
		
		for(const auto& p: paquetes){
			if(strcmp(p->getCodigo(), cod)==0){
				paquetito = p;
				break;
			}
		}
		if(!paquetito)return;
		
		string nombreArch = string(cod) + ".txt";
		
		ofstream archivo(nombreArch);
		if(!archivo.is_open())return;
		
		archivo<<paquetito->getNombre()<<endl;
		
		vector<Dependencia*> deps = Dependencias(cod);
		
		for(const auto& d : deps){
			archivo<<d->getCodDependencia()<<endl;
		}
		
		archivo.close();
	}
	///NO HICE LA SOBRE CARGA DEL OPERADOR << PORQUE LA VERDAD
	///NI ENTENDI DONDE IBA
	
	///CONSIGNA 3
	vector<pair<string, int>>Top5Dependencias(){
		if(paquetes.empty()){
			return {};
		}
		
		vector<pair<string,int>>top5;
		
		for(const auto& p: paquetes){
			top5.push_back({p->getNombre(), p->cantDependencias()});
		}
		
		sort(top5.begin(), top5.end(), [](const auto& a, const auto& b){
			return a.second > b.second;
		});
		
		if(top5.size()>5){
			top5.resize(5);
		}
		
		return top5;
	}
	
	///CONSIGNA 3
	vector<Paquete*>paqueteSinDependencia(){
		if(paquetes.empty()){
			return {};
		}
		
		vector<Paquete*>sinDependencia;
		
		copy_if(paquetes.begin(),paquetes.end(),back_inserter(sinDependencia),[](const auto& a){
			return a->cantDependencias() == 0;
		});
		
		return sinDependencia;
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
	
	T eliminarElemenoto(int indice){
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

///CONSIGNA 5
/*Es necesario utilizar composición cuando trabajamos con clases individuales que se relacionan entre si,
Esta característica nos ayuda a reutilizar código ya que dentro de nuestra clase podemos acceder a objetos
de otras clases y a sus métodos.

El polimorfismo es una técnica de reutilización de código se aplica en clases abstractas o super clases donde, 
las subclases heredan métodos virtuales que también pueden sobrescribir y también atributos de la super clase. */
int main(int argc, char *argv[]) {
	
	return 0;
}

