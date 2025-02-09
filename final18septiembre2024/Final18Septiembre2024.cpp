#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

class Bodega{
protected:
	int nro;
	char tipo;
	int cantidad_litros;
	int anio_produccion;
public:
	Bodega(int nro_, char tipo_, int cantL, int anioP): nro(nro_),tipo(tipo_) ,cantidad_litros(cantL),anio_produccion(anioP){}
	virtual int calcularCosto() const = 0;
	int getAnioProdu(){
		return anio_produccion;
	}
	char getTipo(){
		return tipo;
	}
	int getCantLitros(){
		return cantidad_litros;
	}
	virtual char getEnvace() const = 0;
};

class Mesa: public Bodega{
private:
public:
	
	Mesa(int nro_, char tipo_, int cantL, int anioP): Bodega(nro_,tipo,cantL,anioP){}
	int calcularCosto() const {
		return 120 * this->cantidad_litros;
	}
	char getEnvace() const{
		return 'B';
	}
};

class Premium: public Bodega{
private:
public:
	
	Premium(int nro_, char tipo_, int cantL, int anioP): Bodega(nro_,tipo,cantL,anioP){}
	int calcularCosto() const {
		return 200 * this->cantidad_litros;
	}
	
	char getEnvace() const {
		return 'B';
	}
};

class Especial: public Bodega{
private:
	char tipo_envace;
public:
	Especial(int nro_, char tipo_, int cantL, int anioP): Bodega(nro_,tipo,cantL,anioP){
		tipo_envace = 'B'; ///POR DEFAULT
	}
	int calcularCosto() const {
		return 320 * this->cantidad_litros;
	}
	
	void setTipoEnvace(char tipo){
		this->tipo_envace =  tipo; ///"B" Barrica "A" Botella
	}
	
	char getEnvace()const{
		return tipo_envace;
	}
};

class Gestor{
private:
	vector<Bodega*> vinos;
public:
	///CONSGINA 1 Y 2
	void cargarArchivo(string direccion){
		ifstream archivo(direccion, ios::binary);
		if(archivo.fail()){return;}
		int num,cant,anio;
		char tipo;
		while(archivo.read(reinterpret_cast<char*>(&num),sizeof(num))){
			archivo.read(&tipo, sizeof(tipo));
			archivo.read(reinterpret_cast<char*>(&cant),sizeof(cant));
			archivo.read(reinterpret_cast<char*>(&anio),sizeof(anio));
			switch(tipo){
			case 'M': vinos.push_back(new Mesa(num,tipo,cant,anio));
			break;
			case 'P': vinos.push_back(new Premium(num,tipo,cant,anio));
			break;
			case 'E': vinos.push_back(new Especial(num,tipo,cant,anio));
			break;
			}
		}
	}
	///CONSIGNA 1
	map<string,int> cantLXAnio(int anio, char tipo_vino){
		if(vinos.empty()){return {};}
		
		int precio_final = 0;
		int total_litros = 0;
		
		map<string,int>datosTipoVino;
		
		for(const auto& vino: vinos){
			///CASO MESA
			if(vino->getTipo() == tipo_vino){
				if((anio - vino->getAnioProdu())>= 1){
					precio_final += vino->calcularCosto();
					total_litros += vino->getCantLitros();
				}
			}
			///CASO PREMIUM
			if(vino->getTipo() == tipo_vino){
				if((anio - vino->getAnioProdu())>= 2){
					precio_final += vino->calcularCosto();
					total_litros += vino->getCantLitros();
				}
			}
			///CASO ESPECIAL JAJASJFKAJFKASJ
			if(vino->getTipo() == tipo_vino){
				if(vino->getEnvace() == 'A'){
					if((anio - vino->getAnioProdu())>= 1){
						precio_final += vino->calcularCosto();
						total_litros += vino->getCantLitros();
					}
				}else if(vino->getEnvace() == 'B'){
					if((anio - vino->getAnioProdu())>= 2){
						precio_final += vino->calcularCosto();
						total_litros += vino->getCantLitros();
					}
				}	
			}
			
		}
		datosTipoVino["Precio"]=precio_final;
		datosTipoVino["Litros"]=total_litros;
		
		return datosTipoVino;
	}
	///CONSIGNA 2
	void cargarArchivoTexto(string dirArch){
		ofstream archivo(dirArch);
		if(archivo.fail()){return;}
		archivo<<"2020, 2021, 2022, 2023 :"<<endl;
		for(const auto& vino: vinos){
			if(vino->getAnioProdu()<=2023 && vino->getAnioProdu()>=2020){
				archivo<<"Año: "<<vino->getAnioProdu()<<", Litros: "<<vino->getCantLitros()<<vino->calcularCosto()<<endl;
			}
		}
		archivo.close();
	}
	///CONSIGNA 3
	vector<pair<int,int>> AniosCML(){
		if(vinos.empty()){return {};}
		map<int, int> litrosPorAnio;
		
		
		for (const auto& v : vinos) {
			litrosPorAnio[v->getAnioProdu()] += v->getCantLitros();
		}
		
		vector<pair<int,int>> escandalo(litrosPorAnio.begin(),litrosPorAnio.end());
		
		sort(escandalo.begin(),escandalo.end(),[](const auto& a, const auto &b){
			return a.second > b.second;
		});
		
		if(escandalo.size() > 5){
			escandalo.resize(5);
		}
		
		return escandalo;
	}
	///CONSGINA 3
	int anioMayorVenta(){
		if (vinos.empty()) return -1;
		
		map<int, int>pepe;
		
		for(const auto& v: vinos){
			int anio = v->getAnioProdu();
			if (v->getTipo() == 'M') {
				anio += 1;
			} else if (v->getTipo() == 'P') {
				anio += 2;
			} else if (v->getTipo() == 'E') {
				if (v->getEnvace() == 'A') {
					anio+= 1;
				} else if (v->getEnvace() == 'B') {
					anio += 2;
				}
			}
			pepe[anio]+=v->getCantLitros();
		}
		
		int anioTop = 0;
		int maxLitros = 0;
		for(const auto& p: pepe){
			if(p.second > maxLitros){
				anioTop = p.first;
				maxLitros = p.second;
			}
		}
		
		return anioTop;
	}
	///CONSIGNA 3
	int anioMMV(){
		if(vinos.empty()){return -1;}
		map<int,int>aniomp;
		
		for(const auto& v: vinos){
			aniomp[v->getAnioProdu()]+=v->getCantLitros();
		}
		
		int anioMVP=0;
		int cantML=0;
		
		for(const auto& a: aniomp){
			if(cantML > a.second){
				cantML = a.second;
				anioMVP = a.first;
			}
		}
		
		return anioMVP;
	}
};

///CONSIGNA 4
template <typename T>
struct NodoCola {
	T dato;
	NodoCola* link;
};

template <typename T>
class Cola {
private:
	NodoCola<T>* frente;
	NodoCola<T>* fin;
	
public:
	Cola() : frente(nullptr), fin(nullptr) {}
	
	bool cola_vacia() {
		return frente == nullptr;
	}
	
	void agregarDato(T dato) {
		NodoCola<T>* nuevo_nodo = new NodoCola<T>;
		nuevo_nodo->dato = dato;
		nuevo_nodo->link = nullptr;
		
		if (cola_vacia()) {
			frente = nuevo_nodo;
		} else {
			fin->link = nuevo_nodo;
		}
		fin = nuevo_nodo;
	}
	
	T cola_obtener() {
		if (cola_vacia()) {
			throw runtime_error("Error: La cola está vacía");
		}
		
		NodoCola<T>* aux = frente;
		T dato = aux->dato;
		frente = frente->link;
		
		if (frente == nullptr) {
			fin = nullptr;
		}
		
		delete aux;
		return dato;
	}
	
};

int main(int argc, char *argv[]) {
	
	
	return 0;
}

