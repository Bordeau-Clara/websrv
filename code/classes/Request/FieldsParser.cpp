/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FieldsParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:24:44 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "statusCodes.hpp"
#include <cstdlib>
#include <iostream>

std::string	skipOWS(std::string str) //ne sert plus a rien??
{
	// int cursor = 0;
	// while (str[cursor] && OWS.find(str[cursor]))
	// {
	// 	cursor++;
	// }
	// str.erase(0, cursor);
	return str;
}

void	Request::parseHost(std::string str)
{
	//format: <host>:<port>
	//<port> optional
	this->_host = str;
}

void	Request::parseCookies(std::string str)
{
	//contains stored HTTP cookies associated with the server 
	//(previously sent by the server with the Set-Cookie header
	//or set in JavaSript using Document.cookie)
	//format: name=value; name2=value2; name3=value3
	
	(void)str;
	//see what use for cookie bonus
}

void	Request::parseConnection(std::string str)
{
	//keep-alive ou close
	//DEL ou pas le client apres le traitement de la requete
	if (!str.compare("keep-alive"))
		this->_connection = KEEP_ALIVE;
	if (!str.compare("close"))
		this->_connection = CLOSE;
}

void	Request::parseExpect(std::string str)
{
	this->_expect.assign(str);
}

void	Request::parseContentType(std::string str)
{
	this->_contentType.assign(str);
}

void	Request::parseContentLength(std::string str)
{
	//if chunked and content length -> 400
	if (this->_transferEncoding == CHUNKED)
	{
		this->_status.assign(BAD_REQUEST);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Cannot have Content-Length and Transfer-encoding at the same time"
			<< std::endl;
		return;
	}
	this->_length = 1;
	this->_contentLength = std::strtol(str.c_str(), NULL, 10);
	if (this->_contentLength > MAX_BODY_SIZE)
	{
		this->_status.assign(BAD_REQUEST);
	}
}

void	Request::parseTransferEncoding(std::string str)
{
	//format xxx, xxx
	//encodage dans lequel le body va etre envoyer
	//dans notre cas chunked est le seul a gerer
	//pour les autres types 501 Not Implemented
	//
	//lire ligne: "50\r\n" => taille hex
	//lire 80 bytes de body
	//lire CRLF
	//lire next chunk
	//fin si chunk size = 0
	//lire final CRLF + trailers eventuels
	//reconstruire le body
	//
	//pour cgi pas de VE HTTP_TRANSFER_ENCODING
	//=> trouver la taille du body reconstitue et creer CONTNENT_LENGTH
	//if chunked and content length -> 400
	if (this->_length == 1)
	{
		this->_status.assign(BAD_REQUEST);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Cannot have Content-Length and Transfer-encoding at the same time"
			<< std::endl;
		return;
	}
	if (str.find("chunked") != std::string::npos)
		this->_transferEncoding = CHUNKED;
	else
	{
		this->_status.assign(BAD_REQUEST);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Only accept chunked encoding"
			<< std::endl;
	}
}

void	Request::parseTrailer(std::string str)
{
	(void)str;
	this->_trailer = 1;
}

// void	Request::parseAccept(std::string str)
// {
// 	//content types the sender is able to anderstand
// 	//the server uses content negociation to select one of the proposals
// 	//inform the client with Content-Type response
// 	//usefull only if cgi, accept elements are passed as <CGI spec> argument
// 	//
// 	//format xxx, xxx;q=nb.nb
// 	//format to be checked
// 	//q is between 0 and 1, with a max of 3 decimals, by default 1
// 	std::cout << "Accept is : " << str << std::endl;
// 	str = skipOWS(str);
// }

// void	Request::parseAcceptEncoding(std::string str)
// {
// 	//liste des encodages de contenu que le client accept
// 	//meme format que accept
// 	//format xxx, xxx;q=nb.nb
// 	//inutile pour static
// 	//a passer a la CGI HTTP_ACCEPT_ENCODING
// 	str = skipOWS(str);
// }

// void	Request::parseLanguage(std::string str)
// {
// 	//format xxx, xxx;q=nb.nb
// 	str = skipOWS(str);
// }

// void	Request::parseAuthorization(std::string str)
// {
// 	//header d'authentification
// 	//inutile en static
// 	//a passer a la CGI HTTP_AUTHORIZATION
// 	//trop le bordel cgi se debrouille
// 	str = skipOWS(str);
// }

// void	Request::parseIfModifiedSince(std::string str)
// {
// 	//indique que le client ne veut que les fichiers modifies apres une certaine date
// 	//stat()
// 	//parser la date en time_t
// 	//si time <= if-modified-since -> 304 Not Modified
// 	//CGI HTTP_IF_MODIFIED_SINCE
// 	//format jour, jj, moi AAAA HH:MM:SS TIME_ZONE (ex gmt)
// 	str = skipOWS(str);
// }

//Trailer
//
//indique quels headers apparaitront apres un message chunked
//viennent apres le chunk final (chunk size = 0)
//a stocker qqpart pour pouvoir les supprimer de la fin du body
//ne surtout pas les envoyer a la cgi

//Liste des fields possible a verifier pour la CGI
//pragma
//cache-control
//date
//upgrade (ne pas envoyer a la cgi mais valide)
//accept-charset
//from
//if-mach
//if-none-match
//if-range
//if-unmodified-since
//max-forward (ne pas envoyer mais a valider)
//proxy-authorization (ne pas envoyer mais a valider)
//range
//referer
//TE
//user-agent
//
//lie au body
//content-encoding
//content-location
//content-language
//content-range

//methode -> REQUEST_METHOD
//request-target -> PATH_INFO ou QUERY_STRING


//Variable CGI obligatoire
//REQUEST_METHOD
//REQUEST_URI
//SCRIPT_NAME
//PATH_INFO
//QUERY_STRING
//SERVER_PROTOCOL
//SERVER_NAME ->host
//SERVER_PORT
//REMOTE_ADDR
//
//field du header deviennent
//HTTP_<FIELD_NAME>
//
//exception
//CONTENT_LENGTH
//CONTENT_TYPE
//
//SERVER_NAME ->hostname extrait du Host
//HTTP_HOST ->valeur brut du header Host
