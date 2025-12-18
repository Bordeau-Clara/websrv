/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stateMachine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:10:29 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum ParsingBits
{
	BIT_RW = 1 << 7,
	BIT_CGI = 1 << 6,
	BIT_HB = 1 << 5,
	BIT_CHNKD = 1 << 4,
	BIT_OC = 1 << 3,
	BIT_TRAILER = 1 << 2,
	BIT_EMPTY = 1 << 1, //bit non utilise
	BIT_ERROR = 1,
} ParsingBits;

typedef enum parsing_state
{
	READ = 0,
	EXEC,

	CGI,

	HEADER,
	BODY,

	CHUNKED,

	OCTET,
	CHUNK_SIZE,

	TRAILERS,

	ERROR,
} parsing_state;

// typedef enum parsing_state
// {
// 	INIT = 0,
// 	READ = ~(1 << 7),
// 	SEND = 1 << 7,
// 	CGI = 1 << 6,
// 	HEADER = ~(1 << 5),
// 	BODY = 1 << 5,
// 	OCTET = ~(1 << 4),
// 	CHUNK_SIZE = 1 << 4,
// 	TRAILERS = 1 << 3,
// 	ERROR = 1,
// } parsing_state;
