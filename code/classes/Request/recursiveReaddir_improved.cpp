/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursiveReaddir_improved.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 08:21:10 by aykrifa           #+#    #+#             */
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

// Fonction auxiliaire pour collecter les entrées de manière récursive
void	Request::collectEntries(std::string subPath, std::vector<FileEntry>& entries, std::string currentPrefix)
{
    struct dirent *entry;
    std::string current_path = _requestedRessource + subPath;
    DIR *dir = opendir(current_path.c_str());

    if (dir == NULL)
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        FileEntry fileEntry;
        fileEntry.name = entry->d_name;
        fileEntry.path = subPath + "/" + entry->d_name;
        fileEntry.isDirectory = (entry->d_type == DT_DIR);

        entries.push_back(fileEntry);

        if (entry->d_type == DT_DIR)
        {
            collectEntries(subPath + "/" + entry->d_name, entries, currentPrefix + "/" + entry->d_name);
        }
    }
    closedir(dir);
}

// Fonction pour générer le HTML avec la structure arborescente
void	Request::generateTreeHTML(std::string subPath, int depth, std::string& htmlOutput)
{
    struct dirent *entry;
    std::string current_path = _requestedRessource + subPath;
    DIR *dir = opendir(current_path.c_str());

    if (dir == NULL)
        return;

    std::vector<FileEntry> directories;
    std::vector<FileEntry> files;

    // Séparer fichiers et dossiers
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

    // Afficher les dossiers d'abord
    for (size_t i = 0; i < directories.size(); i++)
    {
        std::string folderPath = directories[i].path;
        std::string displayPath = _location->_name + folderPath;
        trimSlash(displayPath);

        htmlOutput += "<div class=\"folder\" style=\"margin-left: ";
        htmlOutput += nbrToString(depth * 20);
        htmlOutput += "px;\">\n";
        htmlOutput += "  <div class=\"folder-header\" onclick=\"toggleFolder(this)\">\n";
        htmlOutput += "    <span class=\"arrow\">▶</span>\n";
        htmlOutput += "    <span class=\"folder-icon\">📁</span>\n";
        htmlOutput += "    <span class=\"folder-name\">";
        htmlOutput += directories[i].name;
        htmlOutput += "</span>\n";
        htmlOutput += "  </div>\n";
        htmlOutput += "  <div class=\"folder-content\" style=\"display: none;\">\n";

        // Récursion pour le contenu du dossier
        generateTreeHTML(folderPath, depth + 1, htmlOutput);

        htmlOutput += "  </div>\n";
        htmlOutput += "</div>\n";
    }

    // Afficher les fichiers
    for (size_t i = 0; i < files.size(); i++)
    {
        std::string filePath = files[i].path;
        std::string displayPath = _location->_name + filePath;
        trimSlash(displayPath);

        htmlOutput += "<div class=\"file\" style=\"margin-left: ";
        htmlOutput += nbrToString(depth * 20 + 20);
        htmlOutput += "px;\">\n";
        htmlOutput += "  <span class=\"file-icon\">📄</span>\n";
        htmlOutput += "  <a href=\"";
        htmlOutput += displayPath;
        htmlOutput += "\">";
        htmlOutput += files[i].name;
        htmlOutput += "</a>\n";
        htmlOutput += "</div>\n";
    }
}

bool	Request::recursiveReaddir(std::string subPath)
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
    _response.body += "  <title>Index of ";
    _response.body += _location->_name;
    _response.body += "</title>\n";
    _response.body += "  <style>\n";
    _response.body += "    * { margin: 0; padding: 0; box-sizing: border-box; }\n";
    _response.body += "    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #f5f5f5; }\n";
    _response.body += "    .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px 20px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n";
    _response.body += "    .header h1 { font-size: 2.5em; margin-bottom: 10px; text-shadow: 2px 2px 4px rgba(0,0,0,0.2); }\n";
    _response.body += "    .header p { font-size: 1.1em; opacity: 0.9; }\n";
    _response.body += "    .container { max-width: 1200px; margin: 30px auto; padding: 0 20px; }\n";
    _response.body += "    .content-box { background: white; border-radius: 8px; padding: 30px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); }\n";
    _response.body += "    .breadcrumb { margin-bottom: 20px; padding: 10px; background: #f8f9fa; border-radius: 4px; font-size: 0.9em; color: #666; }\n";
    _response.body += "    .folder { margin: 5px 0; }\n";
    _response.body += "    .folder-header { cursor: pointer; padding: 8px 12px; border-radius: 4px; transition: background 0.2s; display: flex; align-items: center; }\n";
    _response.body += "    .folder-header:hover { background: #f0f0f0; }\n";
    _response.body += "    .arrow { display: inline-block; width: 16px; transition: transform 0.2s; font-size: 0.8em; margin-right: 8px; }\n";
    _response.body += "    .arrow.open { transform: rotate(90deg); }\n";
    _response.body += "    .folder-icon { margin-right: 8px; font-size: 1.2em; }\n";
    _response.body += "    .folder-name { font-weight: 600; color: #333; }\n";
    _response.body += "    .folder-content { overflow: hidden; transition: max-height 0.3s ease; }\n";
    _response.body += "    .file { padding: 6px 12px; margin: 2px 0; border-radius: 4px; transition: background 0.2s; display: flex; align-items: center; }\n";
    _response.body += "    .file:hover { background: #f8f9fa; }\n";
    _response.body += "    .file-icon { margin-right: 8px; font-size: 1.1em; }\n";
    _response.body += "    .file a { text-decoration: none; color: #667eea; flex: 1; }\n";
    _response.body += "    .file a:hover { text-decoration: underline; }\n";
    _response.body += "    .empty-message { text-align: center; padding: 40px; color: #999; }\n";
    _response.body += "  </style>\n";
    _response.body += "</head>\n";
    _response.body += "<body>\n";
    
    // Header avec le bandeau
    _response.body += "  <div class=\"header\">\n";
    _response.body += "    <h1>🌐 WEBSERV</h1>\n";
    _response.body += "    <p>Par Clara & Ayoub</p>\n";
    _response.body += "  </div>\n";
    
    _response.body += "  <div class=\"container\">\n";
    _response.body += "    <div class=\"content-box\">\n";
    _response.body += "      <div class=\"breadcrumb\">📂 Index of: ";
    _response.body += _location->_name;
    _response.body += "</div>\n";
    
    // Générer l'arborescence
    std::string treeHTML;
    generateTreeHTML(subPath, 0, treeHTML);
    
    if (treeHTML.empty())
    {
        _response.body += "      <div class=\"empty-message\">📭 Ce dossier est vide</div>\n";
    }
    else
    {
        _response.body += treeHTML;
    }
    
    _response.body += "    </div>\n";
    _response.body += "  </div>\n";
    
    // JavaScript pour l'interactivité
    _response.body += "  <script>\n";
    _response.body += "    function toggleFolder(element) {\n";
    _response.body += "      const content = element.nextElementSibling;\n";
    _response.body += "      const arrow = element.querySelector('.arrow');\n";
    _response.body += "      if (content.style.display === 'none') {\n";
    _response.body += "        content.style.display = 'block';\n";
    _response.body += "        arrow.classList.add('open');\n";
    _response.body += "      } else {\n";
    _response.body += "        content.style.display = 'none';\n";
    _response.body += "        arrow.classList.remove('open');\n";
    _response.body += "      }\n";
    _response.body += "    }\n";
    _response.body += "  </script>\n";
    _response.body += "</body>\n";
    _response.body += "</html>\n";

    return (true);
}
