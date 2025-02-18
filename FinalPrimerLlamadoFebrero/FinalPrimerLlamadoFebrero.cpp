#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

class Comida{
protected:
	char nombre[200];
	int id;
public:
	Comida(const char* nom, int id_){
		strcpy(nombre, nom);
		id = id_;
	}
	
	virtual int getCosto() = 0;
	int getId(){return id;}
	const char* getNombre(){return nombre;}
};

class Ingrediente: public Comida{
private:
	int costo;
public:
	Ingrediente(const char* nom, int id_, int costo_): Comida(nom,id_){
		costo = costo_;
	}
	
	int getCosto(){
		return costo;
	}
};

class Plato: public Comida{
private:
	vector<Comida*>ingredientes;
public:
	Plato(const char* nom, int id_): Comida(nom,id_){}
	void cargarIngrendientes(Ingrediente* ingre){
		ingredientes.push_back(ingre);
	}
	
	int getCosto(){
		int i = 0;
		for(const auto& p : ingredientes){
			i += p->getCosto();
		}
		return i;
	}
	
	vector <Comida*> getIngredientes(){
		return ingredientes;
	}
};

class Gestor{
private:
	vector<Plato*>platos;
	vector<Ingrediente*>ingredientes;
public:
	///CONSIGNA 1
	void cargarPlatos(Plato* comida_){
		platos.push_back(comida_);
	}
	
	///CONSGINA 1
	void cargarIngredientes(Ingrediente* ingredientes_){
		ingredientes.push_back(ingredientes_);
	}
	
	///CONSIGNA 2
	void cargarArchivo(string dir1, string dir2, string dir3){
		ofstream archivoRelacion(dir1, ios::binary);
		ofstream archivoIngrediente(dir2, ios::binary);
		ofstream archivoPlato(dir3, ios::binary);
		
		if(archivoRelacion.fail() || archivoIngrediente.fail() || archivoPlato.fail()){
			return;
		}
		
		for(const auto& i: ingredientes){
			int id = i->getId();
			archivoIngrediente.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam =  strlen(i->getNombre())+1;
			archivoIngrediente.write(reinterpret_cast<const char*>(&tam),sizeof(int));
			archivoIngrediente.write(i->getNombre(),tam);
			int costo = i->getCosto();
			archivoIngrediente.write(reinterpret_cast<const char*>(&costo),sizeof(int));
		}
		
		for(const auto& p: platos){
			int id = p->getId();
			archivoPlato.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam =  strlen(p->getNombre())+1;
			archivoPlato.write(reinterpret_cast<const char*>(&tam),sizeof(int));
			archivoPlato.write(p->getNombre(),tam);
			int costo = p->getCosto();
			archivoPlato.write(reinterpret_cast<const char*>(&costo),sizeof(int));
		}
		
		for(const auto& p: platos){
			int id = p->getId();
			archivoRelacion.write(reinterpret_cast<const char*>(&id), sizeof(int));
			
			int num_ingredientes = p->getIngredientes().size();
			archivoRelacion.write(reinterpret_cast<const char*>(&num_ingredientes), sizeof(int));///ESTO ES PARA CUANDO TOQUE LEER LOS DATOS SABER CUANTOS INGREDIENTES HAY.
			
			for(const auto& i: p->getIngredientes()){
				Ingrediente* ingrediente = dynamic_cast<Ingrediente*>(i);
				if (ingrediente) {
					int id = ingrediente->getId();
					archivoRelacion.write(reinterpret_cast<const char*>(&id), sizeof(int));
				} 
			}
		}
		
		archivoIngrediente.close();
		archivoPlato.close();
		archivoRelacion.close();
	}
	
	///CONSIGNA 3
	vector<Plato*>platosMenorCosto(vector<Plato*>platos_){
		if(platos_.empty()){
			return {};
		}
		
		sort(platos_.begin(),platos_.end(),[](const auto& a, const auto& b){return a->getCosto()<b->getCosto();});
		
		return platos_;
	}
		
	///COSIGNA 3
	void CargarTop5TXT(vector<Plato*>platos_, string dirArch){
		if(platos_.empty()){
			return;
		}
		
		ofstream archivo(dirArch);
		if(archivo.fail()){return;}
		
		vector<Plato*>platosBaratos = platosMenorCosto(platos_);
		
		platosBaratos.resize(5);
		
		for(const auto& p: platosBaratos){
			archivo<<p->getNombre()<<": "<<p->getCosto()<<endl;
		}
		archivo.close();
	}
	
	///CONSIGNA 3
	void platosSinHarina(string dirArch ,vector<Plato*> platoo) {
		if (platoo.empty()) {
			return ;
		}
		
		vector<Plato*> platosSH;
		
		for (const auto& p : platoo) {
			bool tieneHarina = false;
			
			for (const auto& c : p->getIngredientes()) {
				Ingrediente* ingre = dynamic_cast<Ingrediente*>(c);
				if (ingre && strcmp(ingre->getNombre(), "Harina") == 0){
					tieneHarina = true;
					break; 
				}
			}
			
			if (!tieneHarina) {
				platosSH.push_back(p);
			}
		}
		
		ofstream archivo(dirArch);
		if(!archivo.is_open()){return;}
		
		for(const auto& p: platosSH){
			archivo<<"Nombre: "<<p->getNombre()<<"  Costo: "<<p->getCosto()<<endl;
			archivo<<"Ingredientes: "<<endl;
			for(const auto& c: p->getIngredientes()){
				Ingrediente* ingred = dynamic_cast<Ingrediente*>(c);
				if(ingred){
					archivo<<c->getNombre()<<endl;
				}
			}
		}
		archivo.close();
	}	
};

int main(int argc, char *argv[]) {
	
	return 0;
}

