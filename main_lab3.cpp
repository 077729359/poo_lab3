#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Clasa de bază Product
class Product {
protected:
    int id;
    string nume;
    double pret;
    int stocDisponibil;
public:
    Product(int idProd = 0, string n = "Necunoscut", double p = 0.0, int stoc = 0)
        : id(idProd), nume(n), pret(p), stocDisponibil(stoc) {}

    virtual void descrieProdus() const {
        cout << "ID: " << id << ", Nume: " << nume << ", Pret: " << pret
             << " lei, Stoc: " << stocDisponibil << endl;
    }

    virtual double calculeazaPret() const {
        const double TVA = 0.2;  // TVA de 20%
        return pret + (pret * TVA);
    }

    virtual bool afiseazaDisponibilitate() const {
        return stocDisponibil > 0;
    }

    void scadeStoc(int cantitate) {
        if (stocDisponibil >= cantitate) {
            stocDisponibil -= cantitate;
        } else {
            cout << "Nu există suficient stoc pentru " << cantitate << " unități.\n";
        }
    }

    int getId() const {
        return id;
    }

    int getStoc() const {
        return stocDisponibil;
    }

    virtual ~Product() = default;
};

// Clasele derivate care moștenesc virtual din Product
class LampaBirou : virtual public Product {
public:
    LampaBirou(int idProd, string n, double p, int stoc)
        : Product(idProd, n, p, stoc) {}

    void descrieProdus() const override {
        cout << "Lampa de Birou - ";
        Product::descrieProdus();
    }
};

class LampaPodea : virtual public Product {
public:
    LampaPodea(int idProd, string n, double p, int stoc)
        : Product(idProd, n, p, stoc) {}

    void descrieProdus() const override {
        cout << "Lampa de Podea - ";
        Product::descrieProdus();
    }
};

class LampaPerete : virtual public Product {
public:
    LampaPerete(int idProd, string n, double p, int stoc)
        : Product(idProd, n, p, stoc) {}

    void descrieProdus() const override {
        cout << "Lampa de Perete - ";
        Product::descrieProdus();
    }
};

// Clasa Cos care moștenește toate cele 3 clase derivante
class Cos : public LampaBirou, public LampaPodea, public LampaPerete {
private:
    vector<Product*> produse;

public:
    Cos() : Product(0, "Cos", 0.0, 0),
            LampaBirou(0, "Cos", 0.0, 0),
            LampaPodea(0, "Cos", 0.0, 0),
            LampaPerete(0, "Cos", 0.0, 0) {}

    void adaugaInCos(Product* produs, int cantitate) {
        if (produs && produs->afiseazaDisponibilitate()) {
            if (cantitate <= produs->getStoc()) {
                for (int i = 0; i < cantitate; ++i) {
                    produse.push_back(produs);
                }
                produs->scadeStoc(cantitate);
                cout << "Adăugat în coș: ";
                produs->descrieProdus();
            } else {
                cout << "Stoc insuficient pentru " << cantitate << " unități.\n";
            }
        } else {
            cout << "Produsul nu este în stoc!\n";
        }
    }

    void afisareCos() const {
        if (produse.empty()) {
            cout << "Coșul este gol.\n";
            return;
        }
        cout << "\nProduse în coș:\n";
        for (const auto& p : produse) {
            p->descrieProdus();
        }
    }

    double calculeazaTotal() const {
        double total = 0.0;
        for (const auto& p : produse) {
            total += p->calculeazaPret();
        }
        return total;
    }

    void confirmarePlata() {
        double total = calculeazaTotal();
        if (total == 0.0) {
            cout << "Coșul este gol. Nu există nimic de plată.\n";
        } else {
            cout << "Total de plată: " << total << " lei." << endl;
            cout << "Plata a fost efectuată cu succes!" << endl;
            golesteCos();
        }
    }

    void golesteCos() {
        produse.clear();
        cout << "Coșul a fost golit.\n";
    }

    void descrieProdus() const override {
        cout << "Produse din coș:\n";
        for (const auto& p : produse) {
            p->descrieProdus();
        }
    }

    // Căutare produs după ID
    Product* cautaProdusDupaId(int idProdus) {
        for (auto& p : produse) {
            if (p->getId() == idProdus) {
                return p;
            }
        }
        return nullptr;
    }

    ~Cos() override = default;
};

// Funcție pentru a adăuga produs la coș
void adaugaProdusInCos(Cos& cos, vector<Product*>& produse) {
    int idProdus;
    int cantitate;

    cout << "Introduceti ID-ul produsului: ";
    cin >> idProdus;

    // Căutăm produsul după ID
    Product* produs = nullptr;
    for (auto& p : produse) {
        if (p->getId() == idProdus) {
            produs = p;
            break;
        }
    }

    if (produs == nullptr) {
        cout << "Produsul cu ID-ul " << idProdus << " nu a fost găsit.\n";
        return;
    }

    cout << "Introduceti cantitatea dorita: ";
    cin >> cantitate;

    cos.adaugaInCos(produs, cantitate);
}

int main() {
    // Crearea unor produse
    LampaBirou lampa1(1, "Lampa Birou", 100, 10);
    LampaPodea lampa2(2, "Lampa Podea", 200, 5);
    LampaPerete lampa3(3, "Lampa Perete", 150, 8);

    // Stocarea produselor într-un vector
    vector<Product*> produse = {&lampa1, &lampa2, &lampa3};

    // Crearea unui coș de cumpărături
    Cos cos;

    int optiune;
    do {
        cout << "\n*** Meniu Magazin Lămpi ***\n";
        cout << "1. Afișare produse disponibile\n";
        cout << "2. Adăugare produse în coș\n";
        cout << "3. Afișare produse în coș\n";
        cout << "4. Confirmare plată\n";
        cout << "0. Ieșire\n";
        cout << "Alegeți o opțiune: ";
        cin >> optiune;

        switch (optiune) {
            case 1: {
                cout << "\nProduse disponibile:\n";
                for (const auto& produs : produse) {
                    produs->descrieProdus();
                }
                break;
            }
            case 2: {
                int id, cantitate;
                cout << "Introduceti ID-ul produsului: ";
                cin >> id;

                // Căutăm produsul după ID
                Product* produs = nullptr;
                for (auto* p : produse) {
                    if (p->getId() == id) {
                        produs = p;
                        break;
                    }
                }

                if (produs == nullptr) {
                    cout << "Produsul cu ID-ul introdus nu există.\n";
                    break;
                }

                cout << "Introduceți cantitatea dorită: ";
                cin >> cantitate;

                cos.adaugaInCos(produs, cantitate);
                break;
            }
            case 3:
                cos.afisareCos();
                break;
            case 4:
                cos.confirmarePlata();
                break;
            case 0:
                cout << "La revedere!\n";
                break;
            default:
                cout << "Opțiune invalidă!\n";
        }
    } while (optiune != 0);

    return 0;
}
