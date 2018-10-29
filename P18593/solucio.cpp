#include <iostream>
#include <cstddef>
#include <stack>
using namespace std;

template <typename T>
class Arbre {
private:

    Arbre() : _arrel(NULL) {
    };

    struct node {
        T info;
        node* primf;
        node* seggerm;
    };
    node* _arrel;
    static node* copia_arbre(node* p);
    static void destrueix_arbre(node* p) throw ();
    static node* suma_arbres(node *a1, node *a2);

public:
    // Construeix un Arbre format per un únic node que conté a x.
    Arbre(const T &x);

    // Tres grans.
    Arbre(const Arbre<T> &a);
    Arbre& operator=(const Arbre<T> &a);
    ~Arbre() throw ();

    // Col·loca l'Arbre donat com a primer fill de l'arrel de l'arbre sobre el que s'aplica el mètode i l'arbre a queda invalidat; després de fer b.afegir_fill(a), a no és un arbre vàlid.
    void afegir_fill(Arbre<T> &a);

    // Suma d'arbres
    Arbre<T> operator+(const Arbre<T> &a);

    // Iterador sobre arbre general.
    friend class iterador;

    class iterador {
    public:
        friend class Arbre;

        // Construeix un iterador no vàlid.
        iterador() throw ();

        // Retorna el subarbre al que apunta l'iterador; llança un error si l'iterador no és vàlid.
        Arbre<T> arbre() const;

        // Retorna l'element del node al que apunta l'iterador o llança un error si l'iterador no és vàlid.
        T operator*() const;

        // Retorna un iterador al primogenit del node al que apunta; llança un error si l'iterador no és vàlid.
        iterador primogenit() const;

        // Retorna un iterador al següent germà del node al que apunta; llança un error si l'iterador no és vàlid.
        iterador seg_germa() const;

        // Operadors de comparació.

        bool operator==(const iterador &it) const {
            return _p == it._p;
        };

        bool operator!=(const iterador &it) const {
            return _p != it._p;
        };
        static const int IteradorInvalid = 410;

    private:
        Arbre<T>::node* _p;
    };

    // Retorna un iterador al node arrel de l'Arbre (un iterador no vàlid si l'arbre no és vàlid).
    iterador arrel() const throw ();

    // Retorna un iterador no vàlid.
    iterador final() const throw ();

    static const int ArbreInvalid = 400;
};

// La còpia es fa seguint un recorregut en preordre.

template <typename T>
typename Arbre<T>::node* Arbre<T>::copia_arbre(node* p) {
    node* aux = NULL;
    if (p != NULL) {
        aux = new node;
        try {
            aux -> info = p -> info;
            aux -> primf = aux -> seggerm = NULL;
            aux -> primf = copia_arbre(p -> primf);
            aux -> seggerm = copia_arbre(p -> seggerm);
        } catch (...) {
            destrueix_arbre(aux);
        }
    }
    return aux;
}

// La destrucció es fa seguint un recorregut en postordre.

template <typename T>
void Arbre<T>::destrueix_arbre(node* p) throw () {
    if (p != NULL) {
        destrueix_arbre(p -> primf);
        destrueix_arbre(p -> seggerm);
        delete p;
    }
}

// Construcció d'un arbre que conté un sol element x a l'arrel.

template <typename T>
Arbre<T>::Arbre(const T &x) {
    _arrel = new node;
    try {
        _arrel -> info = x;
        _arrel -> seggerm = NULL;
        _arrel -> primf = NULL;
    } catch (...) {
        delete _arrel;
        throw;
    }
}

template <typename T>
Arbre<T>::Arbre(const Arbre<T> &a) {
    _arrel = copia_arbre(a._arrel);
}

template <typename T>
Arbre<T>& Arbre<T>::operator=(const Arbre<T> &a) {
    Arbre<T> tmp(a);
    node* aux = _arrel;
    _arrel = tmp._arrel;
    tmp._arrel = aux;
    return *this;
}

template <typename T>
Arbre<T>::~Arbre() throw () {
    destrueix_arbre(_arrel);
}

template <typename T>
void Arbre<T>::afegir_fill(Arbre<T> &a) {
    if (_arrel == NULL or a._arrel == NULL or
            a._arrel -> seggerm != NULL) {
        throw ArbreInvalid;
    }
    a._arrel -> seggerm = _arrel -> primf;
    _arrel -> primf = a._arrel;
    a._arrel = NULL;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::arrel() const throw () {
    iterador it;
    it._p = _arrel;
    return it;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::final() const throw () {
    return iterador();
}

template <typename T>
Arbre<T>::iterador::iterador() throw () : _p(NULL) {
}

template <typename T>
T Arbre<T>::iterador::operator*() const {
    if (_p == NULL) {
        throw IteradorInvalid;
    }
    return _p -> info;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::iterador::primogenit() const {
    if (_p == NULL) {
        throw IteradorInvalid;
    }
    iterador it;
    it._p = _p -> primf;
    return it;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::iterador::seg_germa() const {
    if (_p == NULL) {
        throw IteradorInvalid;
    }
    iterador it;
    it._p = _p -> seggerm;
    return it;
}

template <typename T>
Arbre<T> Arbre<T>::iterador::arbre() const {
    if (_p == NULL) {
        throw IteradorInvalid;
    }
    Arbre<T> a;
    a._arrel = _p;
    Arbre<T> aux(a);
    a._arrel = NULL;
    return aux;
}

template <typename T>
typename Arbre<T>::node* Arbre<T>::suma_arbres(node *p1, node *p2) {
    node* aux = NULL;
    //si els dos tenen node/valor
    if (p1 != NULL && p2 != NULL) {
        aux = new node;
        aux -> info = p1->info + p2->info;
        cout << " " << aux->info;
        aux -> primf = aux -> seggerm = NULL;
        aux -> primf = suma_arbres(p1 -> primf, p2 -> primf);
        aux -> seggerm = suma_arbres(p1 -> seggerm, p2 -> seggerm);
    }
    if (p1 != NULL && p2 == NULL) {
        aux = new node;
        aux -> info = p1->info;
        cout << " " << aux->info;
        aux -> primf = aux -> seggerm = NULL;
        aux -> primf = suma_arbres(p1 -> primf, NULL);
        aux -> seggerm = suma_arbres(p1 -> seggerm, NULL);
    }
    if (p1 == NULL && p2 != NULL) {
        aux = new node;
        aux -> info = p2->info;
        cout << " " << aux->info;
        aux -> primf = aux -> seggerm = NULL;
        aux -> primf = suma_arbres(NULL, p2 -> primf);
        aux -> seggerm = suma_arbres(NULL, p2 -> seggerm);
    }
    if (p1 == NULL && p2 == NULL) {
        return aux;
    }

    return aux;
}

template <typename T>
Arbre<T> Arbre<T>::operator+(const Arbre<T> &a) {
    Arbre<T> sumat = Arbre();
    node* arrel_suma = suma_arbres(_arrel, a._arrel);
    sumat._arrel = arrel_suma;
    return sumat;
}

//llegeix arbre en preordre

Arbre<int> llegir_arbre() {
    int arrel, fills;
    stack<Arbre<int> > pila;
    cin >> arrel;
    cin >> fills;
    Arbre<int> root(arrel);

    if (fills > 0) {
        for (int i = 0; i < fills; i++) {
            pila.push(llegir_arbre());
            //Arbre<int> fill = llegir_arbre();
            //root.afegir_fill(fill);
        }
    }
    for (int i = 0; i < fills; i++) {
        root.afegir_fill(pila.top());
        pila.pop();
    }

    return root;
}

void print_pos(Arbre<int>::iterador it, Arbre<int>::iterador end) {
    //cout << *it;
    if (it != end) {
        print_pos(it.primogenit(), end);
        cout << " " << *it;
        print_pos(it.seg_germa(), end);
    }
}

void print_pre(Arbre<int>::iterador it, Arbre<int>::iterador end) {
    //cout << *it;
    if (it!=end) {
        cout << " " << *it;
        print_pos(it.primogenit(),end);
        print_pos(it.seg_germa(),end);
    }
}

int main() {
    int nodes;
    cin >> nodes;
    Arbre<int> a = llegir_arbre();
    cin >> nodes;
    Arbre<int> b = llegir_arbre();

    Arbre<int> res = a + b;
    //print_pre(res.arrel(), res.final());
    cout << endl;
    return 0;
}