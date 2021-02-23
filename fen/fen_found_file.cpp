#include "fen_found_file.h"

using namespace mps;
using namespace fenetre;

fen_found_file::~fen_found_file() = default;

QString fen_found_file::exists(bool copy) {
    if(m_file.exists())
        return m_file.name();
    else {
        auto path = found_file_message(QString(tr("Le fichier ")).append(m_file.name())
                                     .append(tr(" est introuvable. Souhaitez-vous le créer ou le trouver?")),
                                     m_file.name(), m_file.extension());
        if(path.isEmpty()) {
            m_file.creer();
            return m_file.name();
        }
        else if(copy) {
            if(m_file.copy(path))
                return m_file.name();
            else {
                /*throw std::runtime_error("Imposible d'effectuer la copie "+path.toStdString()+ "->"
                +m_file.name().toStdString());*/
                QMessageBox::critical(m_modal,tr("Opération impossible"),QString("Imposible d'effectuer la copie ")
                                      .append(path).append("->").append(m_file.name()).append("."));
                return exists(copy);
            }
        }
        else
            return path;
    }
}

QString fen_found_file::is_valid(bool copy) {
    if(m_file.is_valid())
        return m_file.name();
    else {

        auto path = found_file_message(QString(tr("Le fichier ")).append(m_file.name())
                                     .append(tr(" est invalide. Souhaitez-vous le recréer ou en trouver un autre trouver?")),
                                     m_file.name(), m_file.extension());
        if(path.isEmpty()) {
            m_file.creer();
            return m_file.name();
        }
        else if(copy) {
            if(m_file.copy(path))
                return m_file.name();
            else {
           //throw std::runtime_error("Imposible d'effectuer la copie "+path.toStdString()+ "->"+m_file.name().toStdString());
                QMessageBox::critical(m_modal,tr("Opération impossible"),QString(tr("Imposible d'effectuer la copie "))
                                      .append(path).append("->").append(m_file.name()).append("."));
                return is_valid(copy);
            }
        }
        else
            return path;
    }
}

void fen_found_file::open_in_conf(fichier::config &config, const QString &path, bool save) {
    //! Cherche l'entrée path dans le fichier de configuration
    //! et affiche une fenêtre de demande d'ouverture de fichier si l'entrée n'est pas présente.
    QString path_file;
    auto exist = config.var_exists(path);
    if(exist)
        path_file = config.get_vars(path);
    else
        path_file = found_file_message(QString("L'entrée suivante n'existe pas dans le fichier de configuration,\n")
                                    .append(path)
                                    .append("\n Pour que cette entrée soit créée dans le fichier de configuration,"
                                    " souhaitez-vous indiquer un fichier coorespondant ou que celui-ci soit créé?\n"),
                                    m_file.name(),
                                    m_file.extension());


    //! Si le chemin path_file n'est pas vide il est enregistré par le gestionnaire de fichier,
    //! puis on test l'existence, la validité et l'ouverture du fichier.
    //! Ensuite si le chemin est valide, avec l'argument save=true l'entrée est enregistrée dans le fichier de configuration.
    if(!path_file.isNull()) {
        m_file.set_name(path_file);
        if(exist) {
            auto modif = false;
            while(!path_file.isNull() &&!(m_file.exists() &&m_file.is_valid() &&m_file.open())) {
                path_file = found_file_message(QString("L'entrée suivante dans le fichier de configuration est invalide,\n\"")
                                            .append(path)
                                            .append("\". Pour que cette entrée soit corrigé dans le fichier de configuration,"
                                                    " souhaitez-vous indiquer un fichier coorespondant"
                                                    " valide ou que celui-ci soit créé à nouveau?\n"),
                                            m_file.name(),
                                            m_file.extension());
                if(!path_file.isNull())  {
                    m_file.set_name(path_file);
                    modif = true;
                }
            }
            if(modif &&save)
                config.modify(path,path_file);
        }
        else {
            while(!path_file.isNull() &&!(m_file.exists() &&m_file.is_valid() &&m_file.open())) {
                path_file = found_file_message("Le fichier indiqué est invalide, "
                                               "souhaitez-vous indiquer un autre fichier coorespondant valide"
                                               " ou que celui-ci soit créé?\n",
                                               m_file.name(), m_file.extension());
                if(!path_file.isNull())
                    m_file.set_name(path_file);
            }
            if(save &&!path_file.isNull())
                config.add(path,path_file);
        }
    }

    //! Si le chemin est vide, un emplacement pour le nouveau fichier est demandé.
    //! Si le chemin est valide, avec l'argument save=true l'entrée est enregistrée dans le fichier de configuration.
    if(path_file.isNull()) {
        while(path_file.isNull())
            path_file = QFileDialog::getSaveFileName(m_modal,
                                                     "Sous quel nom et à quel emplacement souhaitez-vous crée le fichier?",
                                                     config.default_directory(),
                                                     m_file.extension());
        m_file.set_name(path_file);
        if(m_file.exists())
            QFile::remove(path_file);
        if(m_file.creer() &&save)
            config.add(path,path_file);
        else {
            QMessageBox::critical(m_modal,"Opération impossible","Impossible de créé le fichier" + m_file.name() +".");
            throw std::runtime_error("Impossible de créé le fichier" + m_file.name().toStdString() +".");
        }
    }
}

// %%%%%%%%%%%%%%%%%%%%%% Protected %%%%%%%%%%%%%%%%%%%%%%%%%%%%

QString fen_found_file::found_file_message(const QString &text, const QString &name, const QString &extension,
                                       const QString &accept, const QString &reject, const QString &open_text) {
    if(!message(text,accept,reject)) {
        auto path = QFileDialog::getOpenFileName(m_modal,open_text,name,extension);
        if(path.isEmpty())
            return found_file_message(text,name,extension,accept,reject,open_text);
        else
            return path;
    }
    return QString();
}

bool fen_found_file::message(const QString &text, const QString &accept, const QString &reject) {
   QMessageBox box(m_modal);
   box.setText(text);
   QPushButton buttonAccept(accept,&box);
   box.addButton(&buttonAccept,QMessageBox::AcceptRole);
   QPushButton buttonReject(reject,&box);
   box.addButton(&buttonReject,QMessageBox::RejectRole);
   box.exec();
   return box.clickedButton() == &buttonAccept;
}
