/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:45:29 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/18 15:27:36 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::fields[207][3] = {};
void (Request::*Request::ptr[210])(std::string) = {NULL};

void Request::initFields()
{
	Request::fields[40][0] = "range";
	Request::fields[42][0] = "accept";
	Request::fields[58][0] = "host";
	Request::fields[66][0] = "origin";
	Request::fields[67][0] = "expect";
	Request::fields[70][0] = "cookies";
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

	// Request::ptr[40] = &Request::parseRange;
	Request::ptr[42] = &Request::parseAccept;
	Request::ptr[58] = &Request::parseHost;
	// Request::ptr[66] = &Request::parseOrigin;
	Request::ptr[67] = &Request::parseExpect;
	Request::ptr[70] = &Request::parseCookies;
	Request::ptr[102] = &Request::parseConnection;
	Request::ptr[147] = &Request::parseLanguage;
	Request::ptr[150] = &Request::parseAcceptEncoding;
	// Request::ptr[151] = &Request::parseCacheControl;
	Request::ptr[164] = &Request::parseAuthorization;
	// Request::ptr[187] = &Request::parseIfNoneMatch;
	Request::ptr[189] = &Request::parseContentLength;
	Request::ptr[191] = &Request::parseContentType;
	Request::ptr[201] = &Request::parseTransferEncoding;
	Request::ptr[205] = &Request::parseIfModifiedSince;
}
