#ifndef BIT_MATH_H
#define BIT_MATH_H

// Macro to set a specific bit in a register
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))


// Macro to clear a specific bit in a register
#define CLR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))

// Macro to toggle a specific bit in a register
#define TOG_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))

// Macro to get the value of a specific bit in a register
#define GET_BIT(REG, BIT) (((REG) >> (BIT)) & 1)

#endif // BIT_MATH_H
