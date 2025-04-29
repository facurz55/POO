#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
using namespace std;

class Alumno{
protected:
	int codigo;
	char nya[200];
	vector<int>notas;
public:
	Alumno(int cod, const char* nya_){
		this->codigo = cod;
		strcpy(nya, nya_);
	}
	
	int getCodigo(){return this->codigo;}
	const char* getNya(){return this->nya;}
	vector<int> getNotas(){return this->notas;}
	virtual double calcularProm() = 0;
	virtual void cargarNotas(vector<int> notaas) = 0;
};

class Invitado: public Alumno{
public:
	Invitado(int cod, const char* nya): Alumno(cod, nya){}
	void cargarNotas(vector<int>notaas){
		if(notaas.size() == 1){
			notas.push_back(notaas[0]);
		}else{
			return;
		}
	}
	double calcularProm() {
		return notas[0];
	}
};

class Medio: public Alumno{
public:
	Medio(int cod, const char* nya): Alumno(cod, nya){}
	void cargarNotas(vector<int>notaas){
		
		if(notaas.size() == 3){
			notas.push_back(notaas[0]);
			notas.push_back(notaas[1]);
			notas.push_back(notaas[2]);
		}else{
			return;
		}
		
	}
	
	double calcularProm() override{
		double prom = 0;
		for(int p: notas){
			prom = prom + p;
		}
		prom = prom / 3;
		return prom;
	}
};

class Premium: public Alumno{
public:
	Premium(int cod, const char* nya): Alumno(cod, nya){}
	void cargarNotas(vector<int> notaas){
		if(notaas.size() == 5){
			notas.push_back(notaas[0]);
			notas.push_back(notaas[1]);
			notas.push_back(notaas[2]);
			notas.push_back(notaas[3]);
			notas.push_back(notaas[4]);
		}else{
			return;
		}
	}
	
	double calcularProm(){
		double prom = 0;
		for(int p: notas){
			prom = prom + p;
		}
		prom = prom / 5;
		return prom;
	}
};


class Gestor{
private:
	vector<Alumno*>totalAlumnos;
public:
	
	void cargarAlumno(Alumno* nuevo){
		totalAlumnos.push_back(nuevo);
	}
	///CONSIGNA 1
	void guardarAlumno(string dir, Alumno* nuevo){
		ofstream archivo(dir, ios::binary | ios::app);
		if (archivo.fail()) return;
		
		int cod = nuevo->getCodigo();
		archivo.write(reinterpret_cast<const char*>(&cod), sizeof(cod));
		
		int tam = strlen(nuevo->getNya());
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
		archivo.write(nuevo->getNya(), tam);
		
		vector<int> notas = nuevo->getNotas();
		int cantNotas = notas.size();
		archivo.write(reinterpret_cast<const char*>(&cantNotas), sizeof(cantNotas));
		
		for (int nota : notas) {
			archivo.write(reinterpret_cast<const char*>(&nota), sizeof(nota));
		}
		
		archivo.close();
	}
	///CONSIGNA 2
	void guardarAlumnosBinario(string dirInv, string dirMed, string dirPrem){
		
		if(totalAlumnos.empty()){return;}
		
		for(const auto& p: totalAlumnos){
			if(dynamic_cast<Invitado*>(p)){
				guardarAlumno(dirInv, p);
			}else if(dynamic_cast<Medio*>(p)){
				guardarAlumno(dirMed, p);
			}else if(dynamic_cast<Premium*>(p)){
				guardarAlumno(dirPrem, p);
			}
		}
		
	}
	
	vector<Alumno*> alumnosAprobados(){
		if(totalAlumnos.empty()){return{};}
		
		vector<Alumno*>promocionados;
		
		for(const auto& p: totalAlumnos){
			if(dynamic_cast<Invitado*>(p)){
				for(int n: p->getNotas()){
					if(n > 60){
						promocionados.push_back(p);
					}
				}
			}else if(dynamic_cast<Medio*>(p)){
				int sum = 0;
				
				for(int n: p->getNotas()){ sum = sum + n; }
				
				if((sum/3)>70){ promocionados.push_back(p); }
			}else if(dynamic_cast<Premium*>(p)){
				int sum = 0;
				bool check = true;
				
				for(int n: p->getNotas()){ 
					
					if(n <= 70){
						check = false;
					}
					
					sum = sum + n; 
				}
				
				if(check && (sum/5)>80){ promocionados.push_back(p); }
			}
		}
		
		return promocionados;
	}
	
	///CONSIGNA 3
	void cambiarDePlan(Alumno* user, string tipo){
		if(dynamic_cast<Invitado*>(user)){
			Alumno* nuevo = nullptr;
			int indice = -1;
			
			// Buscar índice del alumno
			for (size_t i = 0; i < totalAlumnos.size(); ++i) {
				if (totalAlumnos[i]->getCodigo() == user->getCodigo()) {
					indice = i;
					break;
				}
			}
			
			if (indice == -1) {
				cout << "No se encontró el alumno!" << endl;
				return;
			}
			
			
			if(tipo == "Medio"){
				nuevo = new Medio(user->getCodigo(),user->getNya());
				delete totalAlumnos[indice];
				totalAlumnos[indice] = nuevo;
				cout<<"Cambio de plan realizado correctamente!!"<<endl;
			}
			else if(tipo == "Premium"){
				nuevo = new Premium(user->getCodigo(),user->getNya());
				delete totalAlumnos[indice];
				totalAlumnos[indice] = nuevo;
				cout<<"Cambio de plan realizado correctamente!!"<<endl;
			}
			else{
				cout<<"El Tipo de plan al que quieres cambiarte no existe"<<endl;
			}
			
		}else{
			cout<<"El usuario no es de tipo Invitado!!";
		}
	}
	
	///Consinga 4
	vector<Alumno*> promedioMasAlto(){
		if(totalAlumnos.empty()){
			return{};
		}
		
		vector<Alumno*>ordenado = totalAlumnos;
		vector<Alumno*>promMasAltos;
		
		sort(ordenado.begin(), ordenado.end(), [](const auto& a, const auto& b){return a->calcularProm() > b->calcularProm();});
		
		for(const auto& p : ordenado){
			if(ordenado[0]->calcularProm() == p->calcularProm()){
				promMasAltos.push_back(p);
			}else{
				break;
			}
		}
		
		return promMasAltos;
	}
	
	map<string, int> alumnosPorTipo() {
		if (totalAlumnos.empty()) return {};
		
		map<string, int> alumXT;
		
		for (const auto& p : totalAlumnos) {
			if (dynamic_cast<Invitado*>(p)) {
				alumXT["Invitado"]++;
			} else if (dynamic_cast<Medio*>(p)) {
				alumXT["Medio"]++;
			} else if (dynamic_cast<Premium*>(p)) {
				alumXT["Premium"]++;
			}
		}
		
		return alumXT;
	}
	
};

int main() {
	Gestor gestor;
	
	// Crear alumnos
	Alumno* a1 = new Invitado(1, "Juan Perez");
	Alumno* a2 = new Medio(2, "Maria Gomez");
	Alumno* a3 = new Premium(3, "Carlos Ruiz");
	
	// Cargar notas
	a1->cargarNotas({85}); // Solo 1 nota
	a2->cargarNotas({70, 75, 80}); // 3 notas
	a3->cargarNotas({90, 85, 95, 88, 87}); // 5 notas
	
	// Cargar en el gestor
	gestor.cargarAlumno(a1);
	gestor.cargarAlumno(a2);
	gestor.cargarAlumno(a3);
	
	// Guardar en archivos binarios
	gestor.guardarAlumnosBinario("invitados.bin", "medios.bin", "premiums.bin");
	
	// Mostrar alumnos con mayor promedio
	auto top = gestor.promedioMasAlto();
	cout << "Alumnos con mayor promedio:" << endl;
	for (Alumno* a : top) {
		cout << "- " << a->getNya() << " con promedio: " << a->calcularProm() << endl;
	}
	
	// Mostrar alumnos aprobados
	auto aprobados = gestor.alumnosAprobados();
	cout << "\nAlumnos aprobados:" << endl;
	for (Alumno* a : aprobados) {
		cout << "- " << a->getNya() << endl;
	}
	
	// Cambiar a1 (Invitado) a tipo Medio
	cout << "\nCambiando plan de Juan Perez a Medio..." << endl;
	gestor.cambiarDePlan(a1, "Medio");
	
	// Mostrar distribución por tipo
	auto porTipo = gestor.alumnosPorTipo();
	cout << "\nCantidad de alumnos por tipo:" << endl;
	for (auto p : porTipo) {
		cout << p.first << ": " << p.second << endl;
	}
	
	return 0;
}


