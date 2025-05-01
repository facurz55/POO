#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
using namespace std;

class Comida{
protected:
	int id;
	char nombre[200];
public:
	Comida(const char* nombre_, int id_): id(id_){strcpy(nombre,nombre_);}
	int getId(){return id;}
	const char* getNombre(){return nombre;}
	virtual double getCosto() = 0;
};

class Plato: public Comida{
private:
	vector<Comida*>ingredientes;
public:
	Plato(const char* nombre_, int id_): Comida(nombre_, id_){}
	
	void ingresarIngre(Comida* nueva){
		ingredientes.push_back(nueva);
	}
	
	double getCosto(){
		double costoTotal = 0;
		for(const auto& p : ingredientes){
			costoTotal = costoTotal + p->getCosto();
		}
		return costoTotal;
	}
	
	vector<Comida*>getIngredientes(){
		return ingredientes;
	}
};

class Ingrediente: public Comida{
private:
	double costo;
public:
	Ingrediente(int costo_, const char* nombre_, int id_): Comida(nombre_, id_), costo(costo_){}
	double getCosto(){return costo;}
	
};


class Gestor{
private:
	vector<Comida*>totalComidas;
public:
	void cargarComidas(Comida* nueva){
		totalComidas.push_back(nueva);
	}
	
	///CONSIGNA 2
	void platosBinario(string dirPlato, Plato* plato){
		ofstream archivo (dirPlato , ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int tam = strlen(plato->getNombre()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(plato->getNombre(), tam);
		
		int id = plato->getId();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		
		double total = plato->getCosto();
		archivo.write(reinterpret_cast<const char*>(&total), sizeof(double));
		
		archivo.close();
	}
	
	void ingredientesBinario(string dirIngre, Comida* ingrediente){
		ofstream archivo (dirIngre, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int tam = strlen(ingrediente->getNombre()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(ingrediente->getNombre(), tam);
		
		int id = ingrediente->getId();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		
		double costo = ingrediente->getCosto();
		archivo.write(reinterpret_cast<const char*>(&costo),sizeof(double));
		
		archivo.close();
	}
	
	void relacionBinario(string dirRela, Plato* platos){
		ofstream archivo(dirRela, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int id = platos->getId();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		
		int cantidad = platos->getIngredientes().size();
		archivo.write(reinterpret_cast<const char*>(&cantidad), sizeof(int));
		
		for(const auto& p: platos->getIngredientes()){
			id = p->getId();
			archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
		}
	}
	
	void cargarArchivosBinarios(string dirPlato, string dirIngre, string dirRela){
		if(totalComidas.empty()){return;}
		
		for(const auto& p: totalComidas){
			if(dynamic_cast<Plato*>(p)){
				Plato* ptrPlato = dynamic_cast<Plato*>(p);
				platosBinario(dirPlato, ptrPlato);
				relacionBinario(dirRela, ptrPlato);
			}else{
				ingredientesBinario(dirIngre, p);
			}
		}
	}
	
	///CONSGINA 3
	void platosXMenorCosto(vector<Plato*>&platos){
		if(platos.empty()){
			return;
		}
		
		sort(platos.begin(),platos.end(),[](const auto& a, const auto& b){return a->getCosto()< b->getCosto();});
	}
	
	void top5MasBaratos(vector<Plato*>platos, string dirArch){
		ofstream archivo(dirArch);
		
		if(archivo.fail()){return;}
		
		vector<Plato*>platobarato = platos;
		
		platosXMenorCosto(platobarato);
		
		platobarato.resize(5);
		
		for(int i = 0; i < 5; i++){
			archivo<<platobarato[i]->getNombre()<<" : "<<platobarato[i]->getCosto()<<endl;
		}
		
		archivo.close();
	}
	
	bool contieneHarina(Comida* comida) {
		// Si es un ingrediente, solo comparás el nombre
		Ingrediente* ing = dynamic_cast<Ingrediente*>(comida);
		if (ing) {
			return strcmp(ing->getNombre(), "harina") == 0;
		}
		
		// Si es un plato, buscar en sus ingredientes
		Plato* plato = dynamic_cast<Plato*>(comida);
		if (plato) {
			for (auto& subIng : plato->getIngredientes()) {
				if (contieneHarina(subIng)) return true;
			}
		}
		
		return false;
	}
	
	void guardarPlatosSinHarina(vector<Plato*> platos, string dirArchivo) {
		ofstream archivo(dirArchivo);
		if (archivo.fail()) return;
		
		for (auto& plato : platos) {
			if (!contieneHarina(plato)) {
				archivo << plato->getNombre() << endl;
			}
		}
		
		archivo.close();
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

///CONSIGNA 1
/* Si se puede utilizar polimorfismo, la ventaja es que las subclases pueden compartir metodos con la clase base gracias a virtual podiendo adaptar
las funciones a su manera*/

///CONSIGNA 2
/*
	La herencia se da cuando tenemos una superclase es decir un clase base con subclases, la herencia permite heredar metodos y atributos de la clase base
	a las subclases.

	La composicion es la forma de instancias un clase dentro de otra por ejemplo instancias la clase comida dentro de la clase plato.

	La reutilizacion de codigo ayuda a tener un codigo mas limpio, legible y sin redundancia.
	Los metodos de reutilzacion de codigo son:
	-Herencia.
	-Composicion.
	-Polimorfismo.
	-Template.

	La relacion entre estas tres el objetivo de optimizar el codigo.
*/
