/**
 * File: life-constants.h
 * ----------------------
 * Defines those constants which are shared by both the
 * life-graphics module and the main life module.
 */

#pragma once

/**
 * Constants
 * ---------
 * Both the graphics module and the main module need to agree
 * that cells of age kMaxAge and above are equally old--at least
 * for display purposes.
 */
const int kMaxAge = 12;

const int dirLen = 8;
const int dr[dirLen] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int dc[dirLen] = {-1,  0,  1, -1, 1, -1, 0, 1};