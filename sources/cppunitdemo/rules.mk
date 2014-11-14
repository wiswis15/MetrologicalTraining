SRCDIR := src
TSTDIR := tst
INCDIR := src tst
OBJDIR := bin
OUTDIR := out

APP := calculator

TSTAPP := calculatortest

COMMON := \
    $(SRCDIR)/Calculator.cpp

SRCS :=\
    $(COMMON) \
    $(SRCDIR)/main.cpp

TSTSRCS :=\
    $(COMMON) \
    $(TSTDIR)/CalculatorTest.cpp \
    $(TSTDIR)/CppUnitWrapper.cpp \
    $(TSTDIR)/mainTest.cpp

TSTLIBS := -lcppunit

