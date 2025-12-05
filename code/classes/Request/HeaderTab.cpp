/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:45:29 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::fields[207][3] = {};
void (Request::*Request::fctField[210])(std::string) = {NULL};

void Request::initFields()
{
	Request::fields[40][0] = "range";
	Request::fields[42][0] = "accept";
	Request::fields[58][0] = "host";
	Request::fields[66][0] = "origin";
	Request::fields[67][0] = "expect";
	Request::fields[70][0] = "cookies";
	Request::fields[76][0] = "trailer";
	Request::fields[102][0] = "connection";
	Request::fields[147][0] = "accept-language";
	Request::fields[150][0] = "accept-encoding";
	Request::fields[150][1] = "cache-control";
	Request::fields[164][0] = "authorization";
	Request::fields[187][0] = "if-none-match";
	Request::fields[189][0] = "content-length";
	Request::fields[191][0] = "content-type";
	Request::fields[201][0] = "transfer-encoding";
	Request::fields[205][0] = "if-modified-since";

	// Request::fctField[40] = &Request::parseRange;
	Request::fctField[42] = &Request::parseAccept;
	Request::fctField[58] = &Request::parseHost;
	// Request::fctField[66] = &Request::parseOrigin;
	Request::fctField[67] = &Request::parseExpect;
	Request::fctField[70] = &Request::parseCookies;
	Request::fctField[76] = &Request::parseTrailer;
	Request::fctField[102] = &Request::parseConnection;
	Request::fctField[147] = &Request::parseLanguage;
	Request::fctField[150] = &Request::parseAcceptEncoding;
	// Request::fctField[151] = &Request::parseCacheControl;
	Request::fctField[164] = &Request::parseAuthorization;
	// Request::fctField[187] = &Request::parseIfNoneMatch;
	Request::fctField[189] = &Request::parseContentLength;
	Request::fctField[191] = &Request::parseContentType;
	Request::fctField[201] = &Request::parseTransferEncoding;
	Request::fctField[205] = &Request::parseIfModifiedSince;
}
