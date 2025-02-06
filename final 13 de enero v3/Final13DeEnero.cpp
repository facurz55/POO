#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
using namespace std;

class Alumno{
protected:
	char nombre[200];
	int codigo;
public:
	Alumno(const char* nomb, int cod): codigo(cod){strcpy(nombre, nomb);}
	virtual int calcularProm() const = 0;
	const char* getNombre() const { return nombre; }
	int getCodigo() const { return codigo; }
	virtual void guardarNotas(ofstream &archivo) = 0;
	virtual const char* getTipo() = 0;
};

class Invitado:public Alumno{
private:
	int nota;
public:
	Invitado(const char* nomb, int cod): Alumno(nomb,cod){};
	void guardarNotas(ofstream &archivo) override{
		archivo.write(reinterpret_cast<const char*>(&nota), sizeof(nota));
	}
	void cargarNotas(int nota_){this->nota = nota_;}
	const char* getTipo(){
		return "Invitado"; 
	}
	int calcularProm() const override{
		return nota;
	}
};

class Medio:public Alumno{
private:
	int notas[3];
public:
	Medio(const char* nomb, int cod): Alumno(nomb,cod){};
	
	void cargarNotas(int nota1, int nota2, int nota3){
		notas[0] = nota1;
		notas[1] = nota2;
		notas[2] = nota3;
	}
	
	void guardarNotas(ofstream &archivo) override{
		archivo.write(reinterpret_cast<const char*>(notas),3*sizeof(int));
	}
	
	const char* getTipo(){
		return "Medio"; 
	}
	
	int calcularProm() const override{
		int p = 0;
		for(int i = 0; i<3; i++){
			p += notas[i];
		}
		return p/3;
	}
};

class Premium:public Alumno{
private:
	int notas[5];
public:
	Premium(const char* nomb, int cod): Alumno(nomb,cod){};
	
	void guardarNotas(ofstream &archivo) override{
		archivo.write(reinterpret_cast<const char*>(notas),5*sizeof(int));
	}
	
	int * getNotas(){
		return notas;
	}
	
	void cargarNotas(int nota1, int nota2, int nota3, int nota4, int nota5){
		notas[0] = nota1;
		notas[1] = nota2;
		notas[2] = nota3;
		notas[3] = nota4;
		notas[4] = nota5;
	}
	
	const char* getTipo(){
		return "Premium"; 
	}
	
	int calcularProm() const override{
		int p = 0;
		for(int i = 0; i<5; i++){
			p += notas[i];
		}
		return p/5;
	}
};

class CursoSys{
private:
	vector<Alumno*>alumnos;
public:
	void cargarAlumnos(Alumno* alumno){
		alumnos.push_back(alumno);
	}
	
	void cargarArchivo(string dirArch){
		if(alumnos.empty()){return;}
		ofstream archivo(dirArch,ios::binary);
		if(archivo.fail()){return;}
		int cant = alumnos.size();
		archivo.write(reinterpret_cast<const char*>(&cant), sizeof(cant));
		for(const auto& a: alumnos){
			archivo.write(a->getNombre(),strlen(a->getNombre())+1);
			int cod = a->getCodigo();
			archivo.write(reinterpret_cast<const char*>(&cod),sizeof(cod));
			a->guardarNotas(archivo);
		}
		archivo.close();
	}
	///CONSIGNA 2
	vector<Alumno*> aprobaronElCurso(){
		if(alumnos.empty()){return {};}
		vector<Alumno*>promocionados;
		for(const auto& a : alumnos){
			if(strcmp(a->getTipo(),"Invitado")==0 && a->calcularProm()>60){
				promocionados.push_back(a);
			}
			else if(strcmp(a->getTipo(),"Medio")==0 && a->calcularProm()>70){
				promocionados.push_back(a);
			}
			else if(strcmp(a->getTipo(),"Premium")==0 && a->calcularProm()>80){
				promocionados.push_back(a);
			}
		}
		return promocionados;
	}
	///CONSIGNA 3
		Alumno* cambiarTipoUsuario(Alumno* alumno, const string& nuevoTipo) {
			if (strcmp(alumno->getTipo(), "Invitado") != 0) {
				cout << "El cambio de tipo solo es posible para usuarios de tipo Invitado." << endl;
				return nullptr;
			}
			
			Alumno* nuevo = nullptr;
			
			if (nuevoTipo == "Medio") {
				nuevo = new Medio(alumno->getNombre(), alumno->getCodigo());
			} else if (nuevoTipo == "Premium") {
				nuevo = new Premium(alumno->getNombre(), alumno->getCodigo());
			} else {
				cout << "Tipo de usuario no v\u00e1lido." << endl;
				return nullptr;
			}
			
			cout << "Usuario cambiado exitosamente a " << nuevoTipo << "." << endl;
			return nuevo;
		}
	
	///CONSIGNA 4
	vector<string> AlumnosPromAlto(){
		if(alumnos.empty()){return {};}
		
		vector<string>promAlto;
		
		auto maximo = max_element(alumnos.begin(),alumnos.end(),[](const auto &a, const auto &b){return a->calcularProm()<b->calcularProm();});
		
		for(const auto& p : alumnos){
			if((*maximo)->calcularProm() == p->calcularProm()){
				promAlto.push_back(p->getNombre());
			}
		}
		
		return promAlto;
	}
	
	///CONSIGNA 4
	map<string, int> alumnosPorTipo(){
		if(alumnos.empty()){return {};}
		map<string,int>apt;
		for(const auto& p : alumnos){
			apt[p->getTipo()]++;
		}
		
		return apt;
	}
};

int main(int argc, char *argv[]) {
	CursoSys curso;
	
	// Crear alumnos
	Invitado* inv1 = new Invitado("Juan", 101);
	Medio* med1 = new Medio("Ana", 102);
	Premium* pre1 = new Premium("Luis", 103);
	Premium* pre2 = new Premium("Pedro", 104);
	Invitado* inv2 = new Invitado("Carlos", 105);
	
	// Cargar notas
	inv1->cargarNotas(85);  // Invitado con promedio 85 (aprueba)
	med1->cargarNotas(90, 80, 85);  // Medio con promedio 85 (aprueba)
	pre1->cargarNotas(95, 100, 90, 85, 95);  // Premium con promedio 93 (aprueba)
	pre2->cargarNotas(50, 60, 55, 40, 45);  // Premium con promedio 50 (NO aprueba)
	inv2->cargarNotas(50);  // Invitado con 50 (NO aprueba)
	
	// Agregar alumnos al curso
	curso.cargarAlumnos(inv1);
	curso.cargarAlumnos(med1);
	curso.cargarAlumnos(pre1);
	curso.cargarAlumnos(pre2);
	curso.cargarAlumnos(inv2);
	
	// *** PROBAR AlumnosPromAlto() ***
	vector<string> mejoresAlumnos = curso.AlumnosPromAlto();
	cout << "Alumnos con el promedio más alto:" << endl;
	for (const auto& nombre : mejoresAlumnos) {
		cout << "- " << nombre << endl;
	}
	
	// *** PROBAR aprobaronElCurso() ***
	vector<Alumno*> aprobados = curso.aprobaronElCurso();
	cout << "\nAlumnos que aprobaron el curso:" << endl;
	for (const auto& alumno : aprobados) {
		cout << "- " << alumno->getNombre() << " (" << alumno->getTipo() << ")" << endl;
	}
	
	// *** PROBAR cambiarTipoUsuario() ***
	cout << "\nCambiando el plan de Juan (Invitado) a Medio..." << endl;
	Alumno* nuevoAlumno = curso.cambiarTipoUsuario(inv1, "Medio");
	if (nuevoAlumno) {
		cout << "Nuevo alumno: " << nuevoAlumno->getNombre() << " (" << nuevoAlumno->getTipo() << ")" << endl;
		delete nuevoAlumno;  // Liberar memoria del nuevo objeto
	}
	
	// *** PROBAR alumnosPorTipo() ***
	map<string, int> alumnosPorTipo = curso.alumnosPorTipo();
	cout << "\nCantidad de alumnos por tipo de plan:" << endl;
	for (const auto& entry : alumnosPorTipo) {
		cout << "- " << entry.first << ": " << entry.second << endl;
	}
	
	// Liberar memoria de alumnos originales
	delete inv1;
	delete med1;
	delete pre1;
	delete pre2;
	delete inv2;
	
	return 0;
}

