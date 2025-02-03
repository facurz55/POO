#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class Viajes{
protected:
	char nombre[200];
	char tipo [100];
	int horas;
	float costo_inicial;
	float costo_final;
public:
	virtual float calcularPrecio()const = 0;
	virtual int calcularTotalHoras() = 0;
	virtual const char* getNombre() const = 0;
	virtual const char* getTipo() const = 0;
	virtual int getHoras() = 0;
	virtual int getCostoInicial() const = 0;
	virtual void cargarEnArchivo(ofstream& archivo) = 0;
};

class Avion: public Viajes{
public:
	
	Avion(char* nombre_, float costo_ini, int horas_){
		strcpy(nombre,nombre_);
		strcpy(tipo, "Avion");
		this->costo_inicial = costo_ini;
		this->horas = horas_ + 2;
		this->costo_final = calcularPrecio();
	}
	
	int getCostoInicial(){return this->costo_inicial;}
	
	float calcularPrecio() const{
		float costo_final = (costo_inicial * 0.20);
		costo_final += costo_inicial;
		return costo_final;
	}	
	
	void cargarEnArchivo(ofstream &archivo){
		archivo.write(this->nombre,sizeof(nombre));
		archivo.write(this->tipo,sizeof(tipo));
		archivo.write(reinterpret_cast<const char*>(&costo_final),sizeof(costo_inicial));
		archivo.write(reinterpret_cast<const char*>(&horas),sizeof(horas));
		archivo.write(reinterpret_cast<const char*>(&costo_final),sizeof(costo_final));
	}
	
	int getHoras(){
		return this->horas;
	}
};

class Colectivo: public Viajes{
protected:
	float kilometros;
public:
	
	Colectivo(char* nombre_, int horas_, int km){
		strcpy(nombre,nombre_);
		strcpy(tipo, "Colectivo");
		this->costo_inicial = 8;
		this->horas = horas_;
		this->kilometros = km;
		this->costo_final = calcularPrecio();
	}

	int getCostoInicial(){return this->costo_inicial;}
	
	float calcularPrecio() const{
		int costo_final = costo_inicial * kilometros;
		return costo_final;
	}

	void cargarEnArchivo(ofstream &archivo){
		archivo.write(this->nombre,sizeof(nombre));
		archivo.write(this->tipo,sizeof(tipo));
		archivo.write(reinterpret_cast<const char*>(&costo_final),sizeof(costo_inicial));
		archivo.write(reinterpret_cast<const char*>(&horas),sizeof(horas));
		archivo.write(reinterpret_cast<const char*>(&costo_final),sizeof(costo_final));
	}
	int getHoras(){
		return this->horas;
	}
};

class Mixto: public Viajes{
private:
	Colectivo* viaje_colectivo;
	Avion* viaje_avion;
	float Costo_IA;
	float Costo_IC;
public:
	Mixto(Colectivo* cole, Avion* ave, char* nombre_){
		strcpy(nombre,nombre_);
		strcpy(tipo, "Mixto");
		viaje_avion = ave;
		viaje_colectivo = cole;
		this->horas = calcularTotalHoras();
		this->costo_final = calcularPrecio();
		Costo_IA = viaje_avion->getCostoInicial();
		Costo_IC = viaje_colectivo->getCostoInicial();
	}
	
	float calcularPrecio() const{
		return viaje_avion->calcularPrecio() + viaje_colectivo->calcularPrecio();
	}
	
	int calcularTotalHoras() override{
		return viaje_avion->calcularTotalHoras() + viaje_colectivo->calcularTotalHoras();
	}
	
	void cargarEnArchivo(ofstream &archivo){
		archivo.write(this->nombre,sizeof(nombre));
		archivo.write(this->tipo,sizeof(tipo));
		archivo.write(reinterpret_cast<const char*>(&Costo_IA),sizeof(Costo_IA));
		archivo.write(reinterpret_cast<const char*>(&Costo_IC),sizeof(Costo_IC));
		archivo.write(reinterpret_cast<const char*>(&horas),sizeof(horas));
		archivo.write(reinterpret_cast<const char*>(&costo_final),sizeof(costo_final));
	}
		
	int getHoras(){
		return this->horas;
	}
};

class Gestor{
private:
	vector<Viajes*>travel;
public:
	///CONSIGNA 1
	void cargarViajes(Viajes* viaje){
		travel.push_back(viaje);
	}
	/// CONSIGNA 2
	void subirArchivo(string dirArch, vector<Viajes*>via){
		if(via.empty()){return;}
		ofstream archivo(dirArch, ios::binary);
		if(archivo.fail()){return;}
		int cantidad = via.size();
		archivo.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
		for(const auto& pepe: via){
			pepe->cargarEnArchivo(archivo);
		}
		archivo.close();
	}
	
	void guardarViajes(){
		vector<Viajes*>allIn[3];
		
		for(const auto& p : travel){
			int i = 0;
			if(dynamic_cast<Avion*>(p)){
				i = 0;
			}
			else if (dynamic_cast<Colectivo*>(p)){
				i = 1;
			}
			else if (dynamic_cast<Mixto*>(p)){
				i = 2;
			}
			allIn[i].push_back(p);
		}
		
		subirArchivo("Avion.dat", allIn[0]);
		subirArchivo("Colectivo.dat", allIn[1]);
		subirArchivo("Mixto.dat", allIn[2]);
	}
	
	///CONSIGNA 3
	vector<Viajes*>top5MasBaratos(vector<Viajes*> &via){
		if(via.empty()){return{};}
		vector<Viajes*>top5;
		sort(via.begin(), via.end(), [](const auto &a, const auto &b) {return a->calcularPrecio() < b->calcularPrecio();});
		for(const auto& p: via){
			top5.push_back(p);
		};
		top5.resize(5);
		return top5;
	}
	
	///CONSGINA 3
	Viajes* viajeConMasTiempo(vector<Viajes*>&via){
		if(via.empty()){return nullptr;}
		sort(via.begin(),via.end(),[](const auto& a, const auto& b){ return a->getHoras() > b->getHoras();});
		return via[0];
	}
	
	///CONSIGNA 3
	vector<Viajes*> viajesEnCiertoTiempo(vector<Viajes*>via, int tiempo){
		if(via.empty()){return {};}
		vector<Viajes*>aprobados;
		copy_if(via.begin(),via.end(), back_inserter(aprobados),[tiempo](const auto& p){return p->getHoras() == tiempo;});
		return aprobados;
	}
	
};


///CONSGINA 4
template<typename k, typename v>
class Conjunto{
private:
	struct Nodo{
		k clave;
		v valor;
		Nodo* siguiente;
	};
	Nodo* cabeza;
	
	Nodo* encontrarNodo(k clave){
		Nodo* actual = cabeza;
		while(actual){
			if(actual->clave == clave){
				return actual;
			}
			actual = actual->siguiente;
		}
		return nullptr;
	}
public:
	
	Conjunto(): cabeza(nullptr){}
	
	bool agregar(k& clave, v& valor){
		if(encontrarNodo(clave)){
			return false;
		}
		Nodo* nuevo = new Nodo;
		nuevo->clave = clave;
		nuevo->valor = valor;
		nuevo->siguiente = cabeza;
		cabeza = nuevo;
		cout<<"Se creo un nodo";
	}
	
	Nodo* obtener(k& clave){
		Nodo* nodo = encontrarNodo(clave);
		if(!nodo){
			throw runtime_error("clave no encontrada pa");
		}
		return nodo;
	}
	
	bool eliminar(k& clave){
		Nodo* actual = cabeza;
		Nodo* anterior = nullptr;
		
		while(actual){
			if(actual->clave == clave){
				if(anterior){
					anterior->siguiente = actual->siguiente;
				}else{
					cabeza = actual->siguiente;
				}
				delete actual;
				return true;
			}
			anterior = actual;
			actual = actual->siguiente;
		}
		return false;
	}
};


///CONSIGNA 5
///¿por qué los template son importantes en un lenguaje de tipado estático?
/*Templates en tipado estático: Son importantes porque permiten definir
funciones y clases genéricas que pueden trabajar con
diferentes tipos de datos sin perder la seguridad de tipo. Esto mejora
la flexibilidad y reusabilidad sin sacrificar el control de tipos. */

///¿qué técnicas de reutilización de código conoce y como reutilizar código? ejemplifique.
/*Herencia: Permite reutilizar código de una clase base en clases derivadas.
Composición: Consiste en incluir objetos de otras clases dentro de una clase para reutilizar funcionalidades.
el polimorfismo también puede considerarse una forma de reutilización de código. Permite
que diferentes clases reutilicen el mismo nombre de método pero con comportamientos
específicos según el tipo de objeto. Esto evita duplicación de código y mejora la flexibilidad, 
ya que se pueden agregar nuevas clases sin modificar el código que ya usa el polimorfismo.*/

int main(int argc, char *argv[]) {
	
	return 0;
}

