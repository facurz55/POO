#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>

using namespace std;

class Examen{
private:
	int nota;
public:
	int getNota(){
		return this->nota;
	}
	void guardarNotas(ofstream&& arch){
		arch.write(reinterpret_cast<const char*>(&nota), sizeof(nota));
	}
};

class Usuario{
private:
	char NyA [200];
	int codigo;
	char tipo [200];
	vector<Examen*> notaExamen;
public:
	vector<Examen*> getExamenes(){
		return this->notaExamen;
	}
	
	char* getTipo(){
		return this->tipo;
	}
		
	
	void cambiarPlan(string nuevoPlan){
		strcpy(tipo,nuevoPlan.c_str());
	}
		
	int calcularProm(){
		if(notaExamen.empty()){return 0;}
		int total = 0;
		for(const auto &e: notaExamen){ total += e->getNota();}
		return total / notaExamen.size();
	}
	
	void guardarUsuario(ofstream& arch){
		arch.write(NyA,sizeof(NyA));
		arch.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
		arch.write(tipo, sizeof(tipo));
		int cant = notaExamen.size();
		arch.write(reinterpret_cast<const char*>(&cant),sizeof(cant));
		for(const auto& n: notaExamen){
			n->guardarNotas(arch);
		}
	}	
};

class Gestor{
private:
	vector<Usuario*>usuarios;
public:
	
	///CONSIGNA 1
	void guardadEntidades(string dirArch){
		ofstream archivo(dirArch, ios::binary);
		if(!archivo.is_open()){return;}
		int cant = usuarios.size();
		archivo.write(reinterpret_cast<const char*>(&cant), sizeof(cant));
		for(const auto& u: usuarios){
			u->guardarUsuario(archivo);
		}
		archivo.close();
	}
	
	///CONSIGNA 2	
	vector<Usuario*> alumnos(vector<Usuario*>& users){
		if(users.empty()){return {};}
		vector<Usuario*>promocionados;
		for(const auto& u: users){
			
			if(strcmp(u->getTipo(),"invitado")==0){
				if(u->calcularProm()>60){
					promocionados.push_back(u);
				}
			}
			
			if(strcmp(u->getTipo(),"medio")==0){
				if(u->calcularProm()>70){
					promocionados.push_back(u);
				}
			}
			
			if(strcmp(u->getTipo(),"premium")==0){
				if(u->calcularProm()>80){
					promocionados.push_back(u);
				}
			}
		}
		
		return promocionados;
	}
	
	///CONSIGNA 3
	void cambiarPlan(Usuario*& user, string plan){
		if(strcmp(user->getTipo(),"invitado")==0){
			
			if(strcmp("medio",plan)==0){
				user->cambiarPlan(plan);
				return;
			}
			
			if(strcmp("premium",plan)==0){
				user->cambiarPlan(plan);
				return;
			}
			
		}else{
			cout<<"Hubo un Error con el Plan."<<endl;
		}
	}
	
	///CONSIGNA 4
	vector<Usuario*>PromMasAlto(vector<Usuario*>&user){
		if(user.empty()){return{};}
		sort(user.begin(),user.end(),[](auto &a, auto &c){return a->calcularProm()>b->calcularProm();});
		return user;
	}
	
	///CONSIGNA 4
	map<string,int>cantXTipo(vector<Usuario*>user){
		if(user.empty()){return{};}
		
		map<string,int>cantTipos;
		
		for(const auto& u: user){
			cantTipos[u->getTipo()]++;
		}
		
		return cantTipos;
	}
}

int main(int argc, char *argv[]) {
	
	return 0;
}

