#include <iostream>
using namespace std;

template<typename K, typename V>
struct nodo_listase {
	K clave;
	V valor;
	nodo_listase* link;
};


template<typename K, typename V>
class Dupla {
private:
	typedef nodo_listase<K, V> Nodo;
	Nodo* cabeza;

	Nodo* buscarNodo(const K& clave) const {
		Nodo* actual = cabeza;
		while (actual != nullptr) {
			if (actual->clave == clave) {
				return actual;
			}
			actual = actual->link;
		}
		return nullptr;
	}
	
public:
	Dupla() : cabeza(nullptr){}
		
		
	void agregar(const K& clave, const V& valor) {
		if (buscarNodo(clave) == nullptr) { 
			Nodo* nuevo = new Nodo;
			nuevo->clave = clave;
			nuevo->valor = valor;
			nuevo->link  = cabeza;
			cabeza = nuevo;
		}
	}

	V obtener(const K& clave) {
		Nodo* encontrado = buscarNodo(clave);
		
		if(encontrado != nullptr){
			V dato = encontrado->valor;
			return dato;
		}
		
		return nullptr;
	}
	
	void eliminar(const K& clave) {
		Nodo* actual = cabeza;
		Nodo* anterior = nullptr;
		
		while (actual != nullptr) {
			if (actual->clave == clave) {
				if (anterior == nullptr) {
					cabeza = actual->link; 
				} else {
					anterior->link = actual->link;
				}
				delete actual;
				return;
			}
			anterior = actual;
			actual = actual->link;
		}
	}
	
};


int main(int argc, char *argv[]) {
	
	return 0;
}

