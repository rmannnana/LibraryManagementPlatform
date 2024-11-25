//Boris Hermann NANA
//DAMIBA Ismael
//2�me ann�e de G�nie Logiciel (Promotion_2022)
//Universit� Virtuelle du Burkina Faso

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Structure pour repr�senter une date
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

// Classe Biblioth�que
class Bibliotheque {
private:
    vector<Abonne> abonnes;
    int prochainIdentifiant = 1; // D�but de l'incr�mentation

public:
    // Ajouter un abonn�
    void ajouterAbonne(string nom, Date dateAdhesion) {
        Abonne nouvelAbonne(nom, prochainIdentifiant++, dateAdhesion);
        abonnes.push_back(nouvelAbonne);
        cout << "Abonn� ajout� avec succ�s : " << nom << " (ID: " << nouvelAbonne.identifiant << ")" << endl;
    }

    // G�rer un emprunt
    void gererEmprunt(int id, string titreLivre, Date dateEmprunt) {
        for (auto& abonne : abonnes) {
            if (abonne.identifiant == id) {
                abonne.historique.push_back(titreLivre + " emprunt� le " +
                    to_string(dateEmprunt.jour) + "/" +
                    to_string(dateEmprunt.mois) + "/" +
                    to_string(dateEmprunt.annee));
                cout << "Livre ajout� � l'historique de " << abonne.nom << "." << endl;
                return;
            }
        }
        cout << "Abonn� non trouv� avec l'ID " << id << "." << endl;
    }

    // Afficher l'historique d'un abonn�
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
        cout << "Abonn� non trouv� avec l'ID " << id << "." << endl;
    }

    // Afficher tous les abonn�s
    void afficherAbonnes() {
        ofstream fichier("abonnes.txt");
        if (!fichier.is_open()) {
            cout << "Erreur : Impossible de cr�er le fichier des abonn�s." << endl;
            return;
        }

        cout << "Liste des abonn�s :" << endl;
        for (const auto& abonne : abonnes) {
            cout << "- " << abonne.nom << " (ID: " << abonne.identifiant << ")" << endl;
            fichier << abonne.identifiant << ";" << abonne.nom << ";"
                << abonne.dateAdhesion.jour << "/"
                << abonne.dateAdhesion.mois << "/"
                << abonne.dateAdhesion.annee << endl;
        }
        fichier.close();
        cout << "La liste des abonn�s a �t� enregistr�e dans le fichier 'abonnes.txt'." << endl;
    }

    // Charger les abonn�s depuis un fichier
    void chargerAbonnesDepuisFichier() {
        ifstream fichier("abonnes.txt");
        if (!fichier.is_open()) {
            cout << "Aucun fichier d'abonn�s trouv�. D�marrage sans donn�es." << endl;
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
        cout << "Donn�es des abonn�s charg�es avec succ�s." << endl;
    }
};

// Fonction principale avec une boucle interactive
int main() {
    Bibliotheque biblio;
    biblio.chargerAbonnesDepuisFichier(); // Charger les donn�es au d�marrage
    bool continuer = true;

    while (continuer) {
        cout << "\n=== Menu ===\n";
        cout << "1. Ajouter un abonn�\n";
        cout << "2. G�rer un emprunt\n";
        cout << "3. Consulter l'historique d'un abonn�\n";
        cout << "4. Afficher tous les abonn�s\n";
        cout << "5. Quitter\n";
        cout << "Votre choix : ";

        int choix;
        cin >> choix;

        switch (choix) {
        case 1: {
            cin.ignore(); // Nettoyer le tampon pour �viter des probl�mes de saisie
            cout << "Entrez le nom de l'abonn� : ";
            string nom;
            getline(cin, nom);

            Date date;
            cout << "Entrez la date d'adh�sion (jour mois ann�e) : ";
            cin >> date.jour >> date.mois >> date.annee;

            biblio.ajouterAbonne(nom, date);
            break;
        }
        case 2: {
            cout << "Entrez l'ID de l'abonn� : ";
            int id;
            cin >> id;

            cin.ignore(); // Nettoyer le tampon pour �viter des probl�mes de saisie
            cout << "Entrez le titre du livre : ";
            string titre;
            getline(cin, titre);

            Date date;
            cout << "Entrez la date d'emprunt (jour mois ann�e) : ";
            cin >> date.jour >> date.mois >> date.annee;

            biblio.gererEmprunt(id, titre, date);
            break;
        }
        case 3: {
            cout << "Entrez l'ID de l'abonn� : ";
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
            cout << "Merci d'avoir utilis� le programme !" << endl;
            continuer = false;
            break;
        }
        default: {
            cout << "Choix invalide. Veuillez r�essayer." << endl;
        }
        }
    }

    return 0;
}
