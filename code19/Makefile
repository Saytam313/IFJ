#           Škola: Brno University of Technology, Faculty of Information Technology
# Název projektu: IFJ18
#       Předměty: IFJ a IAL
#           Část: Scanner
#            Tým: 027
#       Varianta: I
#         Autoři: Aleš Tetur          (xtetur01)
#                 Šimon Matyáš        (xmatya11)
#                 Marek Bahník        (xbahni00)
#                 Aliaksei Slepitsa   (xslepi00)
#
CC= gcc -g
CCFLAGS= -std=c99 -Wall -Wextra -pedantic
LDLIBS= -lm
SRC= $(wildcard *.c) 
OBJ= $(SRC:%.c=%.o)


#linking
ifj19: $(OBJ)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm intr_list.o stack.o built_in.o parser.o main.o symtable.o expression.o scanner.o ifj19
