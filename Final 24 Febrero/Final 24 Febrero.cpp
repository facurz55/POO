#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std; // teoria abajo deel main

class Empleado {
private:
	int id;
	char nombre[100];
	vector<Empleado*> empleados;
	Empleado* jefe;
	
public:
	Empleado(int i, const char* n) {
		id = i;
		strcpy(nombre, n);
		jefe = nullptr;
	}
	void agregarempleado(Empleado* e) {
		empleados.push_back(e);
		e->jefe = this; 
	}
	int getid() {
		return id;
	}
	const char* getnombre() {
		return nombre;
	}
	vector<Empleado*> getempleados() {
		return empleados;
	}
	Empleado* getjefe() {
		return jefe;
	}
};

class Gestor {
private:
	vector<Empleado*> totalempleados;
	vector<Empleado*> jefes;
public:
	//actividad2
	void leerarchivo(string direc) {
		ifstream archivo(direc);
		if (archivo.fail()) {
			return;
		}
		string linea;
		vector<Empleado*> pilajefes;
		while (getline(archivo, linea)) {
			int nivel = 0;
			while (nivel < linea.size() && linea[nivel] == '-') {
				nivel++;
			}
			linea = linea.substr(nivel);
			int pos = linea.find('-');
			int id = stoi(linea.substr(0, pos));
			string nombrestring = linea.substr(pos + 1);
			char nombre[100];
			strncpy(nombre, nombrestring.c_str(), sizeof(nombre) - 1);
			nombre[sizeof(nombre) - 1] = '\0';
			Empleado* nuevo = new Empleado(id, nombre);
			totalempleados.push_back(nuevo);
			if (nivel == 0) {  
				jefes.push_back(nuevo);
				pilajefes.clear();
				pilajefes.push_back(nuevo);
			} else {
				while (pilajefes.size() > nivel) {
					pilajefes.pop_back(); 
				}
				pilajefes.back()->agregarempleado(nuevo);
				pilajefes.push_back(nuevo);
			}
		}
		archivo.close();
	}
	//acctividad3
	Empleado* obtenerresponsable(int id) {
		for (const auto& e : totalempleados) {
			if (e->getid() == id) {
				return e->getjefe();
			}
		}
		return nullptr;
	}
	//activiad4
	void guardararchivos(string direc){
		ofstream archivo(direc, ios::binary);
		if (archivo.fail()){
			return;
		}
		for(const auto& e : totalempleados){
			int id = e->getid();
			archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(e->getnombre());
			archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archivo.write(e->getnombre(), tam);
			int total = e->getempleados().size();
			archivo.write(reinterpret_cast<const char*>(&total), sizeof(int));
		}
		archivo.close();
	}
	//activadad5 - a
	Empleado* obtenermasempleados(){
		if (totalempleados.empty()){
			return nullptr;
		}
		auto maximo = max_element(totalempleados.begin(), totalempleados.end(), [](const auto& a, const auto& b) {
			return a->getempleados().size() < b->getempleados().size();
		});
		return *maximo;
	}
	//b
	vector<Empleado*> obtenerrepetidos() {
		map<int, vector<Empleado*>> mapa;
		vector<Empleado*> repetidos;
		for (const auto& e : totalempleados) {
			mapa[e->getid()].push_back(e);
		}
		for (const auto& p : mapa) {
			if (p.second.size() > 1) {
				repetidos.insert(repetidos.end(), p.second.begin(), p.second.end());
			}
		}
		return repetidos;
	}
	//libre 
	void guardarporjefe(Empleado* jefe) {
		if (jefe->getempleados().empty()) {
			return;
		}
		string direc = "empleado" + to_string(jefe->getid()) + ".dat";
		ofstream archivo(direc, ios::binary);
		if (archivo.fail()) {
			return;
		}
		int id = jefe->getid();
		archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
		int tam = strlen(jefe->getnombre());
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(jefe->getnombre(), tam);
		int total = jefe->getempleados().size();
		archivo.write(reinterpret_cast<const char*>(&total), sizeof(int));
		for (const auto& sub : jefe->getempleados()) {
			int subid = sub->getid();
			archivo.write(reinterpret_cast<const char*>(&subid), sizeof(int));
		}
		archivo.close();
		for (const auto& sub : jefe->getempleados()) {
			guardarporjefe(sub);
		}
	}
	void guardararchivos() {
		for (const auto& j : jefes) { 
			guardarporjefe(j);
		}
	}
};

int main(int argc, char *argv[]) {

	return 0;
}

/// 1) no veo necesario el polimorfismo porque solo se trabaja con empleados sin otro tipo de entidades en las jerarquias, solo hay empleados
///   como no veo la necesidad de tener q manejar distintos tipos de objetos, no vi necesaria ninguna clase base para utilizar polimorfismo.
