#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

class Alumno {
protected:
	char nya[100];
	int codigo;
public:
	virtual int calcularProm() const = 0;
	virtual void guardarEntidad(ofstream &arch) const = 0;
	virtual const char* getTipo() const = 0;  
	virtual const char* getNyA() const = 0;
	virtual int getCodigo() const = 0;
};

class Invitado: public Alumno {
private:
	int nota;
	char tipo[10];
public:
	Invitado() { strcpy(tipo, "invitado"); }
	
	void guardarEntidad(ofstream &arch) const override {
		arch.write(nya, sizeof(nya));
		arch.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
		arch.write(reinterpret_cast<const char*>(&nota), sizeof(nota));
	}
	
	const char* getTipo() const override { return tipo; }
	const char* getNyA() const override { return nya; }
	int getCodigo() const override { return codigo; }
	int calcularProm() const override { return nota; }
};

class Medio: public Alumno {
private:
	int notas[3];
	char tipo[10];
public:
	Medio(const char* nya_, int codigo_) {
		strcpy(nya, nya_);
		this->codigo = codigo_;
		strcpy(tipo, "medio");
	}
	
	void guardarEntidad(ofstream &arch) const override {
		arch.write(nya, sizeof(nya));
		arch.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
		for (int i = 0; i < 3; i++) {
			arch.write(reinterpret_cast<const char*>(&notas[i]), sizeof(notas[i]));
		}
	}
	
	int calcularProm() const override {
		int suma = 0;
		for (int i = 0; i < 3; i++) {
			suma += notas[i];
		}
		return suma / 3;
	}
	
	const char* getTipo() const override { return tipo; }
	const char* getNyA() const override { return nya; }  
	int getCodigo() const override { return codigo; }    
};

class Premium: public Alumno {
private:
	int notas[5];
	char tipo[10];
public:
	Premium(const char* nya_, int codigo_) {
		strcpy(nya, nya_);
		this->codigo = codigo_;
		strcpy(tipo, "Premium");
	}
	
	void guardarEntidad(ofstream &arch) const override {
		arch.write(nya, sizeof(nya));
		arch.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
		for (int i = 0; i < 5; i++) {
			arch.write(reinterpret_cast<const char*>(&notas[i]), sizeof(notas[i]));
		}
	}
	
	int calcularProm() const override {
		int suma = 0;
		for (int i = 0; i < 5; i++) {
			suma += notas[i];
		}
		return suma / 5;
	}
	
	const char* getTipo() const override { return tipo; }
	const char* getNyA() const override { return nya; } 
	int getCodigo() const override { return codigo; }    
};

class Gestor {
private:
	vector<Alumno*> alumnos;
public:
	void guardarEntidadesPorTipo() {
		ofstream archivoInv("invitado.bin", ios::binary);
		ofstream archivoMed("medio.bin", ios::binary);
		ofstream archivoPrem("premium.bin", ios::binary);
		
		if (alumnos.empty()) {
			return;
		}
		
		if (!archivoInv.is_open() || !archivoMed.is_open() || !archivoPrem.is_open()) {
			cout << "Algo fallo!!!!!!!!!" << endl;
			return;
		}
		
		for (const auto& a : alumnos) {
			if (strcmp(a->getTipo(), "invitado") == 0) {
				a->guardarEntidad(archivoInv);
			} else if (strcmp(a->getTipo(), "medio") == 0) {
				a->guardarEntidad(archivoMed);
			} else if (strcmp(a->getTipo(), "premium") == 0) {
				a->guardarEntidad(archivoPrem);
			}
		}
		
		archivoInv.close();
		archivoMed.close();
		archivoPrem.close();
	}
	
	Alumno* cambiarPlan(Alumno* alum, const char* nuevoPlan) {
		if (strcmp(alum->getTipo(), "invitado") == 0) {
			if (strcmp(nuevoPlan, "medio") == 0) {
				return new Medio(alum->getNyA(), alum->getCodigo());
			}
			if (strcmp(nuevoPlan, "Premium") == 0) {
				return new Premium(alum->getNyA(), alum->getCodigo());
			}
		}
		cout << "No se puede cambiar el plan. Solo los invitados pueden cambiar a 'medio' o 'premium'." << endl;
		return nullptr;
	}
};

int main() {
	return 0;
}

