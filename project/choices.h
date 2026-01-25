/**
 * @file choices.h
 * @author Dejana Smiljanic
 * @date 14.01.2026.
 * @brief Macro definitions for branch prediction configuration.
 *
 * This header provides a wrapper macro for expression evaluation hints.
 * It allows the developer to suggest to the compiler whether a specific
 * condition is likely to result in a true or false value.
 */

#ifndef CHOICES_H_
#define CHOICES_H_

/**
 * @brief Evaluates an expression with a compiler hint.
 *
 * Depending on the global @ph EXPRS definition, this macro wraps the
 * input expression @p _x with either an expected true or expected false hint.
 *
 *
 * @param _x The boolean expression to be analyzed by the compiler.
 * @return The result of the expression @p _x.
 */

#if EXPRS == 1
#define EXPRA(_x) expected_true(_x)
#elif EXPRS == 0
#define EXPRA(_x) expected_false(_x)
#endif

#endif /* CHOICES_H_ */
