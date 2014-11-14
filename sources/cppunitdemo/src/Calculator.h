#ifndef	_CALCULATOR_H
#define	_CALCULATOR_H


class Calculator
{
public:
	Calculator (void);
	~Calculator (void);

public:
	int Addition (int arg1, int arg2);
	int Subtract (int arg1, int arg2);
	int Multiply (int arg1, int arg2);
	int Divide   (int arg1, int arg2);
	int Power    (int arg1, int arg2);
};

#endif	// _CALCULATOR_H
