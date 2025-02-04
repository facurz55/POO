#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>

using namespace std;

class Permiso{
private:
	char nombrePer[200];
public:
	Permiso(char* nombre_){
		strcpy(nombrePer,nombre_);
	}
	
	const char* getNombre(){
		return this->nombrePer;
	}
};

class Cargo{
private:
	char nombre[200];
	vector<Permiso*>permisos;
public:
	Cargo(char* nombre_, Permiso* per){
		strcpy(nombre,nombre_);
		permisos.push_back(per);
	}
	
	const char* getNombre(){
		return this->nombre;
	}
	
	vector<Permiso*> getPermisos(){
		return this->permisos;
	}
};

class Usuario{
private:
	char nombre[200];
	int cod;
	vector<Cargo*>cargos;
public:
	Usuario(char* nombre_, int cod_, Cargo* cargo_){
		strcpy(this->nombre,nombre_);
		this->cod = cod_;
		cargos.push_back(cargo_);
	};
	
	void agregarCargos(Cargo* cargo_){
		cargos.push_back(cargo_);
	};
	
	const char* getNombre(){
		return this->nombre;
	}
	
	int getCodigo(){
		return this->cod;
	}
	
	vector<Cargo*> getCargos(){
		return this->cargos;
	}
};

class Sugsys{
private:
	vector<Usuario*>usuarios;
public:
	
	///Consigna 1
	void cargarEntidades(Usuario* user){
		this->usuarios.push_back(user);
	}
	///Consigna 1
	void guardarUsuarios(string dirArch) {
		if (usuarios.empty()) { return; }
		
		ofstream archivo(dirArch, ios::binary);
		if (archivo.fail()) { return; }
		
		// Guardar la cantidad de usuarios primero
		int cantUsuarios = usuarios.size();
		archivo.write(reinterpret_cast<const char*>(&cantUsuarios), sizeof(cantUsuarios));
		
		for (const auto& u : usuarios) {
			// Guardar nombre del usuario
			archivo.write(u->getNombre(), strlen(u->getNombre()) + 1);
			
			// Guardar código del usuario
			int codigo = u->getCodigo();
			archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
			
			// Guardar cantidad de cargos
			int cantCargos = u->getCargos().size();
			archivo.write(reinterpret_cast<const char*>(&cantCargos), sizeof(cantCargos));
			
			for (const auto& c : u->getCargos()) {
				// Guardar nombre del cargo
				archivo.write(c->getNombre(), strlen(c->getNombre()) + 1);
				
				// Guardar cantidad de permisos
				int cantPermisos = c->getPermisos().size();
				archivo.write(reinterpret_cast<const char*>(&cantPermisos), sizeof(cantPermisos));
				
				for (const auto& p : c->getPermisos()) {
					// Guardar nombre del permiso
					archivo.write(p->getNombre(), strlen(p->getNombre()) + 1);
				}
			}
		}
		
		archivo.close();
	}
	
	///CONSIGNA 2
	bool tienePermiso(int cod, char* permiso){
		if(usuarios.size()){return false;}
		for(const auto& u: usuarios){
			if(u->getCodigo()==cod){
				for(const auto& c: u->getCargos()){
					for(const auto& p: c->getPermisos()){
						if(strcmp(p->getNombre(),permiso)==0){
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	
	///CONSIGNA 3
	vector<string> cargosCMP(){///CMP = CON MAS PERMISOS
		if(usuarios.empty()){return {};}
		
		vector<pair<string, int>>cantidades;
		
		for(const auto& u: usuarios){
			for(const auto& c: u->getCargos()){
				int cantPer = c->getPermisos().size();
				cantidades.push_back({c->getNombre(), cantPer});
			}
		}
		
		if(cantidades.empty()) return {};
		
		sort(cantidades.begin(),cantidades.end(),[](const auto&a, const auto&b){
			return a.second > b.second;
		});
		
		int maxPermisos = cantidades.front().second; ///CHAVALES USAR FRONT ES LO MISMO QUE USAR [0], YO ME HAGO EL EXOTICO NOMAS.
		
		vector<string>losMasCabrones;
		
		for(const auto& c: cantidades){
			if(c.second == maxPermisos){
				losMasCabrones.push_back(c.first);
			}else{
				break;
			}
		}
		
		return losMasCabrones;
	}
	
	///CONSGINA 3
	vector<pair<string,int>> permisosEVC(){///EVC= PERMISOS EN VARIOS CARGOS
		if(usuarios.empty()){
			return {};
		}
		
		map<string,int>permisos;
		
		for(const auto& u: usuarios){
			for(const auto& c: u->getCargos()){
				for(const auto& p: c->getPermisos()){
					permisos[p->getNombre()]++;
				}
			}
		}
		
		vector<pair<string,int>>permisosOrdenados;
		for(const auto& p: permisos){
			if(p.second > 1){
				permisosOrdenados.push_back({p.first,p.second});
			}
		}
		
		sort(permisosOrdenados.begin(), permisosOrdenados.end(),[](const auto &a, const auto &b){
			return a.second > b.second;
		});
		
		return permisosOrdenados;
	}
	
	///CONSIGNA 3
	vector<string>usuariosCMP(){///CMP = CON MAS PERMISOS
		if(usuarios.empty()){
			return {};
		}
		
		map<string,int>ucmp;
		
		for(const auto& u: usuarios){
			for(const auto& c: u->getCargos()){
				for(const auto& p: c->getPermisos()){
					ucmp[u->getNombre()]++;
				}
			}
		}
		
		int maximo = 0;
		for(const auto& p: ucmp){
			maximo = max(maximo,p.second);
		}
		
		vector<string>maspermisos;
		for(const auto& p: ucmp){
			if(maximo == p.second){
				maspermisos.push_back(p.first);
			}
		}
		
		return maspermisos;
	}
};

///CONSIGNA 4
/*¿Por qué son necesarias las clases template en los lenguajes de tipado estático?
RTA: Son necesarias porque permiten escribir código genérico sin depender de un tipo 
específico, evitando duplicación y mejorando reutilización y flexibilidad en compilación.

¿Por qué C++ utiliza punteros para implementar el polimorfismo?
RTA: C++ usa punteros porque el polimorfismo requiere acceso dinámico a funciones virtuales */

int main(int argc, char *argv[]) {
	
	return 0;
}

