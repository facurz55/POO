#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>


using namespace std;

class Alumno{
protected:
	char nya[100];
	int codigo;
	struct data_alumno{
		char nya[100];
		int codigo;
		vector<int>notas;
	};
public:
	virtual void guardarEntidad(ofstream& arch) const = 0;
	virtual  data_alumno obtenerDatos() const = 0;
	virtual const char* getTipo() const = 0;
	virtual int calcularPromedio() const = 0;
};

class Invitado: public Alumno{
private:
	int notas;
public:
	data_alumno obtenerDatos()const override{
		data_alumno alum;
		strcpy(alum.nya,this->nya);
		alum.codigo = this->codigo;
		alum.notas.push_back(notas);
		return alum;
	}
	
	void guardarEntidad(ofstream& arch)const override{
		data_alumno alum = obtenerDatos();
		arch.write((char*)&alum.nya, sizeof(alum.nya));
		arch.write((char*)&alum.codigo, sizeof(alum.codigo));
		
		size_t sizeNotas = alum.notas.size();
		arch.write((char*)&sizeNotas, sizeof(sizeNotas));
		arch.write((char*)alum.notas.data(), sizeNotas * sizeof(int));
	}
	
	const char* getTipo() const override{
		return "Invitado";
	}
	
	int calcularPromedio() const override{
		return notas;
	}
};

class Medio: public Alumno{
private:
	int notas[3];
public:
	data_alumno obtenerDatos()const override{
		data_alumno alum;
		strcpy(alum.nya,this->nya);
		alum.codigo = this->codigo;
		for(int i = 0; i<3; i++){
			alum.notas.push_back(notas[i]);
		}
		return alum;
	}
	
	void guardarEntidad(ofstream& arch)const override{
		data_alumno alum = obtenerDatos();
		arch.write((char*)&alum.nya, sizeof(alum.nya));
		arch.write((char*)&alum.codigo, sizeof(alum.codigo));
		
		size_t sizeNotas = alum.notas.size();
		arch.write((char*)&sizeNotas, sizeof(sizeNotas));
		arch.write((char*)alum.notas.data(), sizeNotas * sizeof(int));
	}
	
	const char* getTipo() const override{
		return "Medio";
	}
};

class Premium: public Alumno{
protected:
	int notas[5];
public:
	data_alumno obtenerDatos()const override{
		data_alumno alum;
		strcpy(alum.nya,this->nya);
		alum.codigo = this->codigo;
		for(int i = 0; i<5; i++){
			alum.notas.push_back(notas[i]);
		}
		return alum;
	}
	
	void guardarEntidad(ofstream& arch)const override{
		data_alumno alum = obtenerDatos();
		arch.write((char*)&alum.nya, sizeof(alum.nya));
		arch.write((char*)&alum.codigo, sizeof(alum.codigo));
		
		size_t sizeNotas = alum.notas.size();
		arch.write((char*)&sizeNotas, sizeof(sizeNotas));
		arch.write((char*)alum.notas.data(), sizeNotas * sizeof(int));
	}
	
	const char* getTipo() const override{
		return "Premium";
	}
};

class gestor{
private:
	vector<Alumno*>alumnos;
public:
	
	///CONSIGNA 1
	void guardarEntidades(string dirArch){
		ofstream archivo(dirArch, ios::binary);
		if(archivo.fail()){return;}
		for(const auto& alum: alumnos){
			alum->guardarEntidad(archivo);
		}
		archivo.close();
	}
	
	
};

int main(int argc, char *argv[]) {
	
	return 0;
}
