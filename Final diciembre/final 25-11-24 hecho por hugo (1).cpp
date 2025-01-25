#include <algorithm>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <fstream>
using namespace std;

class permiso{
protected:
    char nombre[200];
public:
    const char* getNombreP()const{return nombre;}
    void guardarPermiso(ofstream & out){
        out.write(nombre,sizeof(nombre));
    }
};
class cargo{
protected:
    char nombre[200];
    vector<permiso*> permisos;
public:
    const char* getNombreC()const{return nombre;}
    void mostrarPermisos(){
        for(auto & a : permisos){
            cout<<a->getNombreP()<<endl;
        }
    }
    vector<permiso*> getPermisos(){return this->permisos;}
    void guardar(ofstream& out) const {
        out.write(nombre, sizeof(nombre));
        int cantidadPermisos = permisos.size();
        out.write(reinterpret_cast<const char*>(&cantidadPermisos), sizeof(cantidadPermisos));
        for (const auto& p : permisos) {
            p->guardarPermiso(out);
        }
    }
};
class usuario{
protected:
    char nombre[200];
    int codigo;
    vector<cargo*> cargos;
public:
    usuario(){};
    const char* getNombreU()const{return nombre;}
    void mostrarCargos(){
        cout<<"Usuario: "<< getNombreU()<<endl;
        for(auto & a : cargos){
            cout<<a->getNombreC()<<endl;
            a->mostrarPermisos();
        }
    }
    vector<cargo*> getCargos(){return this->cargos;}
    void guardar(ofstream& out) const {
        out.write(nombre, sizeof(nombre));
        out.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
        int cantCargos = cargos.size();
        out.write(reinterpret_cast<const char*>(&cantCargos), sizeof(cantCargos));
        for (const auto& c : cargos) {
            c->guardar(out);
        }
    }
};
class empresa{
protected:
    vector<usuario*> usuarios;
public:
    void guardarEntidad(const string& dirArch) {
        ofstream arch(dirArch, ios::binary);
        if(!arch.is_open()) {return;}
        int cantidadUsuarios = usuarios.size();
        arch.write(reinterpret_cast<const char*>(&cantidadUsuarios), sizeof(cantidadUsuarios));
        for (const auto& u : usuarios) {u->guardar(arch);}
        arch.close();
    }
    bool checkUserPerm(char* nombreU, char* nombreP){
        bool check=false;
        if(usuarios.empty()){return check;}
        for(auto & usuario : usuarios){
            if(strcmp(usuario->getNombreU(),nombreU)==0){
                for(auto & cargo : usuario->getCargos()){
                    for(auto & permiso : cargo->getPermisos()){
                        if(strcmp(permiso->getNombreP(),nombreP)==0){
                            check=true;
                        }
                    }
                }
            }
        }
        return check;
    }
    vector<cargo*> cargosConMasPermisos(vector<cargo*>& todosLosCargos){
        if(todosLosCargos.empty()){return todosLosCargos;}
        sort(todosLosCargos.begin(),todosLosCargos.end(),[](auto& a, auto& b){
            return a->getPermisos().size() > b->getPermisos().size();
        });
        return todosLosCargos;
    }
    vector<string> permisosComunes(const vector<cargo*>& totalCargos) {
        if (totalCargos.empty()){return {};}
        map<string, int> permisoConteo; // Cambiar a map<string, int>
        for(auto& cargo : totalCargos) {
            for(auto& p : cargo->getPermisos()) {
                permisoConteo[p->getNombreP()]++; // Conversión implícita de char* a string
            }
        }
        vector<string> comunes;
        int numCargos = totalCargos.size();
        for(auto& [permiso, conteo] : permisoConteo) {
            if(conteo == numCargos) {comunes.push_back(permiso);
            }
        }
        return comunes;
    }
    vector<usuario*> masPermisos(vector<usuario*> users) {
        if(users.empty()) {return {};}
        map<string, int> usuarioConteo;
        for(auto& user : users) {
            for(auto& c : user->getCargos()) {
                usuarioConteo[user->getNombreU()] += c->getPermisos().size();
            }
        }
        vector<pair<usuario*, int>> usuariosConPermisos;
        for (auto& user : users) {
            usuariosConPermisos.push_back({user, usuarioConteo[user->getNombreU()]});
        }
        sort(usuariosConPermisos.begin(), usuariosConPermisos.end(),
             [](const pair<usuario*, int>& a, const pair<usuario*, int>& b) {
                 return a.second > b.second;
             });
        vector<usuario*> usuariosOrdenados;
        for (auto& [user, -] : usuariosConPermisos) {
            usuariosOrdenados.push_back(user);
        }
        usuariosOrdenados.resize(5);
        return usuariosOrdenados;
    }

};




/*
4)a)Las clases template son necesarias en los lenguajes de tipado estatico para poder establecer funciones
comunes a todos los tipos de dato y asi reutilizar codigo, es decir que uno puede hacer
la funcion de suma en general, y segun el tipo de dato puede concatenarse,
sumarse normal(algebraicamente hablando) o dar un true/false.
b)En c++ los punteros se utilizan para implementar el polimorfismo porque permiten trabajar con referencias dinámicas a objetos,
lo cual es esencial para resolver problemas relacionados con el comportamiento de clases derivadas en un contexto de clases base.

*/



int main()
{
    cout << "Hello World!" << endl;
    return 0;
}

