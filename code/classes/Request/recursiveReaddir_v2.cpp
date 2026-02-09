/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursiveReaddir_v2.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 08:21:10 by aykrifa           #+#    #+#             */
/*   Updated: 2026/02/09 13:24:16 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "Request.hpp"
#include "Location.hpp"
#include "helpers.hpp"

// Structure pour stocker les informations d'un fichier/dossier
struct FileEntry
{
    std::string name;
    std::string path;
    bool isDirectory;
};

// Fonction pour générer la sidebar avec l'arborescence des dossiers
void Request::generateSidebarHTML(std::string subPath, int depth, std::string& htmlOutput)
{
    struct dirent *entry;
    std::string current_path = _requestedRessource + subPath;
    DIR *dir = opendir(current_path.c_str());

    if (dir == NULL)
        return;

    std::vector<FileEntry> directories;

    // Collecter uniquement les dossiers
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (entry->d_type == DT_DIR)
        {
            FileEntry fileEntry;
            fileEntry.name = entry->d_name;
            fileEntry.path = subPath + "/" + entry->d_name;
            fileEntry.isDirectory = true;
            directories.push_back(fileEntry);
        }
    }
    closedir(dir);

    // Générer les entrées de dossiers
    for (size_t i = 0; i < directories.size(); i++)
    {
        std::string folderPath = directories[i].path;
        std::string folderId = folderPath;
        // Remplacer les / par des _ pour l'ID
        for (size_t j = 0; j < folderId.length(); j++)
        {
            if (folderId[j] == '/')
                folderId[j] = '_';
        }

        htmlOutput += "<div class=\"sidebar-folder\" style=\"margin-left: ";
        htmlOutput += nbrToString(depth * 15);
        htmlOutput += "px;\">\n";
        htmlOutput += "  <div class=\"sidebar-folder-header\" onclick=\"showFolder('";
        htmlOutput += folderId;
        htmlOutput += "')\">\n";
        htmlOutput += "    <span class=\"sidebar-icon\">📁</span>\n";
        htmlOutput += "    <span class=\"sidebar-folder-name\">";
        htmlOutput += directories[i].name;
        htmlOutput += "</span>\n";
        htmlOutput += "  </div>\n";
        htmlOutput += "</div>\n";

        // Récursion pour les sous-dossiers
        generateSidebarHTML(folderPath, depth + 1, htmlOutput);
    }
}

// Fonction pour générer le contenu de tous les dossiers (caché par défaut)
void Request::generateAllFoldersContent(std::string subPath, std::string& htmlOutput)
{
    struct dirent *entry;
    std::string current_path = _requestedRessource + subPath;
    DIR *dir = opendir(current_path.c_str());

    if (dir == NULL)
        return;

    std::vector<FileEntry> directories;
    std::vector<FileEntry> files;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        FileEntry fileEntry;
        fileEntry.name = entry->d_name;
        fileEntry.path = subPath + "/" + entry->d_name;
        fileEntry.isDirectory = (entry->d_type == DT_DIR);

        if (fileEntry.isDirectory)
            directories.push_back(fileEntry);
        else
            files.push_back(fileEntry);
    }
    closedir(dir);

    // Créer le conteneur pour ce dossier
    std::string folderId = subPath;
    if (folderId.empty())
        folderId = "root";
    else
    {
        // Remplacer les / par des _ pour l'ID
        for (size_t j = 0; j < folderId.length(); j++)
        {
            if (folderId[j] == '/')
                folderId[j] = '_';
        }
    }

    std::string displayStyle = (subPath.empty()) ? "block" : "none";
    
    htmlOutput += "<div id=\"";
    htmlOutput += folderId;
    htmlOutput += "\" class=\"folder-content\" style=\"display: ";
    htmlOutput += displayStyle;
    htmlOutput += ";\">\n";

    std::string folderTitle = subPath.empty() ? "." : subPath;
    htmlOutput += "  <h2 class=\"folder-title\">";
    htmlOutput += folderTitle;
    htmlOutput += "</h2>\n";

    if (files.empty() && directories.empty())
    {
        htmlOutput += "  <div class=\"empty-folder\">📭 Dossier vide</div>\n";
    }
    else
    {
        htmlOutput += "  <div class=\"files-grid\">\n";
        
        // Afficher les fichiers
        for (size_t i = 0; i < files.size(); i++)
        {
            std::string filePath = files[i].path;
            std::string displayPath = _location->_name + filePath;
            trimSlash(displayPath);

            htmlOutput += "    <div class=\"file-card\">\n";
            htmlOutput += "      <div class=\"file-icon-large\">📄</div>\n";
            htmlOutput += "      <a href=\"";
            htmlOutput += displayPath;
            htmlOutput += "\" class=\"file-link\">";
            htmlOutput += files[i].name;
            htmlOutput += "</a>\n";
            htmlOutput += "    </div>\n";
        }

        htmlOutput += "  </div>\n";
    }

    htmlOutput += "</div>\n";

    // Récursion pour générer le contenu des sous-dossiers
    for (size_t i = 0; i < directories.size(); i++)
    {
        generateAllFoldersContent(directories[i].path, htmlOutput);
    }
}

bool Request::recursiveReaddir(std::string subPath)
{
    // Vérifier si le chemin peut être ouvert
    std::string current_path = _requestedRessource + subPath;
    DIR *dir = opendir(current_path.c_str());
    
    if (dir == NULL)
        return (false);
    closedir(dir);

    // Générer le header HTML
    _response.body += "<!DOCTYPE html>\n";
    _response.body += "<html lang=\"fr\">\n";
    _response.body += "<head>\n";
    _response.body += "  <meta charset=\"UTF-8\">\n";
    _response.body += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    _response.body += "  <title>Index - ";
    _response.body += _location->_name;
    _response.body += "</title>\n";
    _response.body += "  <style>\n";
    _response.body += "    * { margin: 0; padding: 0; box-sizing: border-box; }\n";
    _response.body += "    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #f8f9fa; display: flex; flex-direction: column; min-height: 100vh; }\n";
    _response.body += "    .header { background: linear-gradient(135deg, #2c3e50 0%, #3498db 100%); color: white; padding: 20px 30px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }\n";
    _response.body += "    .header h1 { font-size: 1.8em; font-weight: 600; }\n";
    _response.body += "    .main-container { display: flex; flex: 1; }\n";
    _response.body += "    .sidebar { width: 280px; background: white; border-right: 1px solid #dee2e6; padding: 20px; overflow-y: auto; box-shadow: 2px 0 5px rgba(0,0,0,0.05); }\n";
    _response.body += "    .sidebar h3 { font-size: 1.1em; margin-bottom: 15px; color: #495057; border-bottom: 2px solid #3498db; padding-bottom: 8px; }\n";
    _response.body += "    .sidebar-folder { margin: 3px 0; }\n";
    _response.body += "    .sidebar-folder-header { cursor: pointer; padding: 8px 10px; border-radius: 4px; transition: all 0.2s; display: flex; align-items: center; }\n";
    _response.body += "    .sidebar-folder-header:hover { background: #e9ecef; }\n";
    _response.body += "    .sidebar-folder-header.active { background: #3498db; color: white; }\n";
    _response.body += "    .sidebar-icon { margin-right: 8px; font-size: 1.1em; }\n";
    _response.body += "    .sidebar-folder-name { font-size: 0.95em; font-weight: 500; }\n";
    _response.body += "    .content-area { flex: 1; padding: 30px; overflow-y: auto; }\n";
    _response.body += "    .folder-content { display: none; }\n";
    _response.body += "    .folder-title { font-size: 1.5em; color: #2c3e50; margin-bottom: 20px; padding-bottom: 10px; border-bottom: 2px solid #e9ecef; }\n";
    _response.body += "    .files-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(200px, 1fr)); gap: 20px; }\n";
    _response.body += "    .file-card { background: white; border: 1px solid #dee2e6; border-radius: 8px; padding: 20px; text-align: center; transition: all 0.3s; box-shadow: 0 1px 3px rgba(0,0,0,0.08); }\n";
    _response.body += "    .file-card:hover { transform: translateY(-4px); box-shadow: 0 4px 12px rgba(0,0,0,0.15); border-color: #3498db; }\n";
    _response.body += "    .file-icon-large { font-size: 3em; margin-bottom: 12px; }\n";
    _response.body += "    .file-link { text-decoration: none; color: #2c3e50; font-weight: 500; word-break: break-word; display: block; }\n";
    _response.body += "    .file-link:hover { color: #3498db; }\n";
    _response.body += "    .empty-folder { text-align: center; padding: 60px 20px; color: #adb5bd; font-size: 1.1em; }\n";
    _response.body += "    .footer { background: #2c3e50; color: white; text-align: center; padding: 20px; margin-top: auto; }\n";
    _response.body += "    .footer p { font-size: 0.9em; opacity: 0.9; }\n";
    _response.body += "    .root-folder { background: #e3f2fd; border-left: 3px solid #3498db; }\n";
    _response.body += "  </style>\n";
    _response.body += "</head>\n";
    _response.body += "<body>\n";
    
    // Header avec le nom de la ressource
    _response.body += "  <div class=\"header\">\n";
    _response.body += "    <h1>📂 ";
    _response.body += _location->_name;
    _response.body += "</h1>\n";
    _response.body += "  </div>\n";
    
    _response.body += "  <div class=\"main-container\">\n";
    
    // Sidebar avec la liste des dossiers
    _response.body += "    <div class=\"sidebar\">\n";
    _response.body += "      <h3>Dossiers</h3>\n";
    
    // Ajouter l'entrée racine
    _response.body += "      <div class=\"sidebar-folder\">\n";
    _response.body += "        <div class=\"sidebar-folder-header root-folder active\" onclick=\"showFolder('root')\">\n";
    _response.body += "          <span class=\"sidebar-icon\">🏠</span>\n";
    _response.body += "          <span class=\"sidebar-folder-name\">. (racine)</span>\n";
    _response.body += "        </div>\n";
    _response.body += "      </div>\n";
    
    // Générer la sidebar
    std::string sidebarHTML;
    generateSidebarHTML(subPath, 0, sidebarHTML);
    _response.body += sidebarHTML;
    
    _response.body += "    </div>\n";
    
    // Zone de contenu principale
    _response.body += "    <div class=\"content-area\">\n";
    
    // Générer le contenu de tous les dossiers
    std::string contentHTML;
    generateAllFoldersContent(subPath, contentHTML);
    _response.body += contentHTML;
    
    _response.body += "    </div>\n";
    _response.body += "  </div>\n";
    
    // Footer
    _response.body += "  <div class=\"footer\">\n";
    _response.body += "    <p>Generated by Clara & Ayoub - WebServ</p>\n";
    _response.body += "  </div>\n";
    
    // JavaScript pour l'interactivité
    _response.body += "  <script>\n";
    _response.body += "    function showFolder(folderId) {\n";
    _response.body += "      // Masquer tous les contenus de dossiers\n";
    _response.body += "      const allContents = document.querySelectorAll('.folder-content');\n";
    _response.body += "      allContents.forEach(content => content.style.display = 'none');\n";
    _response.body += "      \n";
    _response.body += "      // Retirer la classe active de tous les headers\n";
    _response.body += "      const allHeaders = document.querySelectorAll('.sidebar-folder-header');\n";
    _response.body += "      allHeaders.forEach(header => header.classList.remove('active'));\n";
    _response.body += "      \n";
    _response.body += "      // Afficher le contenu sélectionné\n";
    _response.body += "      const selectedContent = document.getElementById(folderId);\n";
    _response.body += "      if (selectedContent) {\n";
    _response.body += "        selectedContent.style.display = 'block';\n";
    _response.body += "      }\n";
    _response.body += "      \n";
    _response.body += "      // Ajouter la classe active au header cliqué\n";
    _response.body += "      event.currentTarget.classList.add('active');\n";
    _response.body += "    }\n";
    _response.body += "  </script>\n";
    _response.body += "</body>\n";
    _response.body += "</html>\n";

    return (true);
}
