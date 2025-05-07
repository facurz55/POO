#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <map>
#include <set>
using namespace std;

class Cargo{
private:
	char nombre[200];
	vector<string>permisos;
public:
	const char* getNombre(){return nombre;}
	vector<string>getPermisos(){return permisos;}
	void addPermisos(string nuevo){ permisos.push_back(nuevo);}
};

class Usuario{
private:
	char nombre[200];
	int codigo;
	vector<Cargo*>cargos;
public:
	const char* getNombre(){return nombre;}
	int getCodigo(){return codigo;}
	vector<Cargo*> getCargos(){return cargos;}
	void addCargo(Cargo* nuevo){cargos.push_back(nuevo);}

};

class Gestor{
private:
	vector<Usuario*>usuarios;
	vector<Cargo*>cargos;
public:
	///CONSIGNA 1)
	void cargarUsuarios(Usuario* nuevo){
		usuarios.push_back(nuevo);
	}
	
	void cargarCargos(Cargo* nuevo){
		cargos.push_back(nuevo);
	}
	
	void usuarioBinario(string dirArch, Usuario* user){
		ofstream archivo(dirArch, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int tam = strlen(user->getNombre()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam),sizeof(int));
		archivo.write(user->getNombre(), tam);
		
		int cod = user->getCodigo();
		archivo.write(reinterpret_cast<const char*>(&cod),sizeof(int));
		
		int cant = user->getCargos().size();
		archivo.write(reinterpret_cast<const char*>(&cant),sizeof(int));
		
		for(const auto& p: user->getCargos()){
			tam = strlen(p->getNombre()) + 1;
			archivo.write(reinterpret_cast<const char*>(&tam),sizeof(int));
			archivo.write(p->getNombre(), tam);
		}
		
		archivo.close();
	}	
	
	void cargoBinario(string dirArch, Cargo* cargo){
		ofstream archivo(dirArch, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int tam = strlen(cargo->getNombre())+1;
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(cargo->getNombre(),tam);
		
		int cant = cargo->getPermisos().size();
		archivo.write(reinterpret_cast<const char*>(&cant), sizeof(int));
		
		for(const auto& p: cargo->getPermisos()){
			tam = p.size() + 1;
			archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archivo.write(p.c_str(),tam);
		}
		
		archivo.close();
	}	
	
	void cargarArchivos(string dirUsurio, string dirCargo){
		if(usuarios.empty() && cargos.empty()){
			return;
		}
		
		for(const auto& p: usuarios){
			usuarioBinario(dirUsurio,p);
		}
		
		for(const auto& p: cargos){
			cargoBinario(dirCargo,p);
		}
	}
	
	///COSNIGNA 2
	bool tienePermiso(int codigo, string permiso){
		for(const auto& p: usuarios){
			if(codigo == p->getCodigo()){
				for(const auto& c: p->getCargos()){
					for(const auto& n: c->getPermisos()){
						if(permiso == n){
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	
	///CONSIGNA 3
	vector<Cargo*> CargosConMasPermisos(){
		if(cargos.empty()){
			return{};
		}
		
		vector<Cargo*>cargosOrdenados = cargos;
		
		sort(cargosOrdenados.begin(), cargosOrdenados.end(), [](const auto& a, const auto&b){return a->getPermisos().size() > b->getPermisos().size();});
		
		int contador = 0;
		
		for(const auto& p: cargosOrdenados){
			if(cargosOrdenados[0]->getPermisos().size() == p->getPermisos().size()){
				contador++;
			}else{
				break;
			}
		}
		
		cargosOrdenados.resize(contador);
		
		return cargosOrdenados;
	}
	
	map<string,int> permisosEnVariosCargos(){
		if(cargos.empty()){
			return {};
		}
		
		map<string, int> permisoConteo;
		
		for (const auto& cargo : cargos) {
			for (const auto& permiso : cargo->getPermisos()) {
				permisoConteo[permiso]++;
			}
		}
		
		map<string, int> permisosRepetidos;
		for (const auto& p : permisoConteo) {
			if (p.second > 1) {
				permisosRepetidos[p.first] = p.second;
			}
		}
		
		return permisosRepetidos;
		
	}
	
	vector<Usuario*> usuariosConMasPermisos() {
		if (usuarios.empty()) return {};
		
		map<Usuario*, int> permisosPorUsuario;
		int maxPermisos = 0;
		
		for (Usuario* u : usuarios) {
			set<string> permisosUnicos;
			
			for (Cargo* c : u->getCargos()) {
				for (const string& permiso : c->getPermisos()) {
					permisosUnicos.insert(permiso);
				}
			}
			
			int cantidad = permisosUnicos.size();
			permisosPorUsuario[u] = cantidad;
			
			if (cantidad > maxPermisos) {
				maxPermisos = cantidad;
			}
		}
		
		// Filtrar los que tienen el máximo
		vector<Usuario*> resultado;
		for (const auto& p : permisosPorUsuario) {
			if (p.second == maxPermisos) {
				resultado.push_back(p.first);
			}
		}
		
		return resultado;
	}
};


///COSNIGNA 5
/*
¿Por qué son necesarias las clases template en los lenguajes de tipado estático?
Las clases template son necesarias en lenguajes de tipado estático como C++ porque permiten definir clases y funciones genéricas que funcionan 
con cualquier tipo de dato. Esto evita la duplicación de código para cada tipo (por ejemplo, int, double, string...), lo que mejora la reutilización, 
legibilidad y mantenimiento del código. Al compilar, el compilador genera versiones específicas de la clase o función para cada tipo utilizado.

¿Por qué C++ utiliza punteros para implementar el polimorfismo?
C++ utiliza punteros (o referencias) para implementar el polimorfismo dinámico porque solo a través de ellos se puede acceder 
a la tabla virtual (vtable) que permite las llamadas a métodos virtuales en tiempo de ejecución. Esto permite que, al usar un puntero a la clase base,
se pueda invocar un método que será resuelto en la subclase correspondiente, logrando así el comportamiento polimórfico.

*/
int main(int argc, char *argv[]) {
	
	
	return 0;
}

