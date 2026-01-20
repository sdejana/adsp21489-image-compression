/*
 * choices.h
 *
 *  Created on: 14.01.2026.
 *      Author: 23dej
 */

#ifndef CHOICES_H_
#define CHOICES_H_

	#if EXPRS == 1
	#define EXPRA(_x) expected_true(_x)
	#elif EXPRS == 0
	#define EXPRA(_x) expected_false(_x)
	#endif;

#endif CHOICES_H_/* CHOICES_H_ */
