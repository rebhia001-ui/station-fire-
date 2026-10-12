#include "connexion.h"
#include "employe.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

bool Connexion::ouvrir(QString &erreur)
{
    // ---- SQLite (provisoire, en attendant Oracle) ----
    const QString dossier = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dossier);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dossier + "/caserne.db");

    // ---- Oracle (à activer plus tard via ODBC) ----
    // QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    // db.setDatabaseName("NOM_DU_DSN");
    // db.setUserName("utilisateur");
    // db.setPassword("motdepasse");

    if (!db.open()) {
        erreur = "Impossible d'ouvrir la base : " + db.lastError().text();
        return false;
    }

    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS employes("
           "id TEXT PRIMARY KEY, nom TEXT NOT NULL, telephone TEXT NOT NULL, email TEXT NOT NULL UNIQUE, "
           "grade TEXT, poste TEXT, disponibilite TEXT, mot_de_passe TEXT NOT NULL, "
           "age INTEGER, heures INTEGER, conges INTEGER, alerte INTEGER DEFAULT 0)");
    q.exec("CREATE TABLE IF NOT EXISTS demandes_conge("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, id_employe TEXT NOT NULL, nom_employe TEXT, "
           "details TEXT, jours INTEGER, statut TEXT, "
           "FOREIGN KEY(id_employe) REFERENCES employes(id) ON DELETE CASCADE)");
    q.exec("PRAGMA foreign_keys = ON");
    q.exec("CREATE INDEX IF NOT EXISTS idx_employes_nom ON employes(nom)");

    // Données de départ (mot de passe de tous les comptes : 1234)
    q.exec("SELECT COUNT(*) FROM employes");
    if (q.next() && q.value(0).toInt() == 0) {
        const QString pw = Employe::hash("1234");
        const QVector<Employe> depart = {
            {"E001", "Sami Gharbi", "20 123 456", "sami.gharbi@email.com", "Sergent", "Chef d'équipe", "En intervention", pw, 34, 52, 4, false},
            {"E002", "Rim Trabelsi", "22 456 789", "rim.trabelsi@email.com", "Caporal", "Pompier", "Disponible", pw, 28, 32, 18, false},
            {"E003", "Karim Majri", "98 321 654", "karim.majri@email.com", "Lieutenant", "Chef de caserne", "Disponible", pw, 41, 49, 12, false},
            {"E004", "Leila Mansouri", "55 987 321", "leila.mansouri@email.com", "Caporal", "Opérateur de dispatch", "Indisponible", pw, 30, 38, 0, false},
            {"E005", "Admin AXIS", "71 000 000", "admin@axis.com", "Capitaine", "Administrateur", "Disponible", pw, 45, 40, 20, false},
        };
        for (Employe e : depart) e.ajouter();
    }

    // Mise à jour pour les bases créées avec une ancienne version
    q.exec("UPDATE employes SET poste='Opérateur de dispatch' WHERE poste='Opérateur'");
    q.exec("SELECT COUNT(*) FROM employes WHERE poste='Magasinier'");
    if (q.next() && q.value(0).toInt() == 0) {
        Employe m;
        m.id = "E006"; m.nom = "Youssef Ben Salah"; m.telephone = "24 111 222"; m.email = "youssef.bensalah@email.com";
        m.grade = "Caporal"; m.poste = "Magasinier"; m.disponibilite = "Disponible";
        m.motDePasse = Employe::hash("1234"); m.age = 36; m.heures = 40; m.conges = 15;
        m.ajouter();
    }
    return true;
}
