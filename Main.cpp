//Boris Hermann NANA
//DAMIBA Ismael
//2ème année de Génie Logiciel (Promotion_2022)
//Université Virtuelle du Burkina Faso

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Structure pour représenter une date
struct Date {
    int jour;
    int mois;
    int annee;
};

// Classe Abonne
class Abonne {
public:
    string nom;
    int identifiant;
    Date dateAdhesion;
    vector<string> historique;

    Abonne(string n, int id, Date date) : nom(n), identifiant(id), dateAdhesion(date) {}
};

// Classe Bibliothèque
class Bibliotheque {
private:
    vector<Abonne> abonnes;
    int prochainIdentifiant = 1; // Début de l'incrémentation

public:
    // Ajouter un abonné
    void ajouterAbonne(string nom, Date dateAdhesion) {
        Abonne nouvelAbonne(nom, prochainIdentifiant++, dateAdhesion);
        abonnes.push_back(nouvelAbonne);
        cout << "Abonné ajouté avec succès : " << nom << " (ID: " << nouvelAbonne.identifiant << ")" << endl;
    }

    // Gérer un emprunt
    void gererEmprunt(int id, string titreLivre, Date dateEmprunt) {
        for (auto& abonne : abonnes) {
            if (abonne.identifiant == id) {
                abonne.historique.push_back(titreLivre + " emprunté le " +
                    to_string(dateEmprunt.jour) + "/" +
                    to_string(dateEmprunt.mois) + "/" +
                    to_string(dateEmprunt.annee));
                cout << "Livre ajouté à l'historique de " << abonne.nom << "." << endl;
                return;
            }
        }
        cout << "Abonné non trouvé avec l'ID " << id << "." << endl;
    }

    // Afficher l'historique d'un abonné
    void afficherHistorique(int id) {
        for (const auto& abonne : abonnes) {
            if (abonne.identifiant == id) {
                cout << "Historique d'emprunt pour " << abonne.nom << " (ID: " << id << ") :" << endl;
                for (const auto& entry : abonne.historique) {
                    cout << "- " << entry << endl;
                }
                return;
            }
        }
        cout << "Abonné non trouvé avec l'ID " << id << "." << endl;
    }

    // Afficher tous les abonnés
    void afficherAbonnes() {
        ofstream fichier("abonnes.txt");
        if (!fichier.is_open()) {
            cout << "Erreur : Impossible de créer le fichier des abonnés." << endl;
            return;
        }

        cout << "Liste des abonnés :" << endl;
        for (const auto& abonne : abonnes) {
            cout << "- " << abonne.nom << " (ID: " << abonne.identifiant << ")" << endl;
            fichier << abonne.identifiant << ";" << abonne.nom << ";"
                << abonne.dateAdhesion.jour << "/"
                << abonne.dateAdhesion.mois << "/"
                << abonne.dateAdhesion.annee << endl;
        }
        fichier.close();
        cout << "La liste des abonnés a été enregistrée dans le fichier 'abonnes.txt'." << endl;
    }

    // Charger les abonnés depuis un fichier
    void chargerAbonnesDepuisFichier() {
        ifstream fichier("abonnes.txt");
        if (!fichier.is_open()) {
            cout << "Aucun fichier d'abonnés trouvé. Démarrage sans données." << endl;
            return;
        }

        abonnes.clear();
        string ligne;
        while (getline(fichier, ligne)) {
            size_t pos1 = ligne.find(';');
            size_t pos2 = ligne.find(';', pos1 + 1);
            size_t pos3 = ligne.find('/', pos2 + 1);
            size_t pos4 = ligne.find('/', pos3 + 1);

            int id = stoi(ligne.substr(0, pos1));
            string nom = ligne.substr(pos1 + 1, pos2 - pos1 - 1);
            Date date = { stoi(ligne.substr(pos2 + 1, pos3 - pos2 - 1)),
                         stoi(ligne.substr(pos3 + 1, pos4 - pos3 - 1)),
                         stoi(ligne.substr(pos4 + 1)) };

            abonnes.push_back(Abonne(nom, id, date));
            prochainIdentifiant = max(prochainIdentifiant, id + 1);
        }
        fichier.close();
        cout << "Données des abonnés chargées avec succès." << endl;
    }
};

// Fonction principale avec une boucle interactive
int main() {
    Bibliotheque biblio;
    biblio.chargerAbonnesDepuisFichier(); // Charger les données au démarrage
    bool continuer = true;

    while (continuer) {
        cout << "\n=== Menu ===\n";
        cout << "1. Ajouter un abonné\n";
        cout << "2. Gérer un emprunt\n";
        cout << "3. Consulter l'historique d'un abonné\n";
        cout << "4. Afficher tous les abonnés\n";
        cout << "5. Quitter\n";
        cout << "Votre choix : ";

        int choix;
        cin >> choix;

        switch (choix) {
        case 1: {
            cin.ignore(); // Nettoyer le tampon pour éviter des problèmes de saisie
            cout << "Entrez le nom de l'abonné : ";
            string nom;
            getline(cin, nom);

            Date date;
            cout << "Entrez la date d'adhésion (jour mois année) : ";
            cin >> date.jour >> date.mois >> date.annee;

            biblio.ajouterAbonne(nom, date);
            break;
        }
        case 2: {
            cout << "Entrez l'ID de l'abonné : ";
            int id;
            cin >> id;

            cin.ignore(); // Nettoyer le tampon pour éviter des problèmes de saisie
            cout << "Entrez le titre du livre : ";
            string titre;
            getline(cin, titre);

            Date date;
            cout << "Entrez la date d'emprunt (jour mois année) : ";
            cin >> date.jour >> date.mois >> date.annee;

            biblio.gererEmprunt(id, titre, date);
            break;
        }
        case 3: {
            cout << "Entrez l'ID de l'abonné : ";
            int id;
            cin >> id;

            biblio.afficherHistorique(id);
            break;
        }
        case 4: {
            biblio.afficherAbonnes();
            break;
        }
        case 5: {
            cout << "Merci d'avoir utilisé le programme !" << endl;
            continuer = false;
            break;
        }
        default: {
            cout << "Choix invalide. Veuillez réessayer." << endl;
        }
        }
    }

    return 0;
}
