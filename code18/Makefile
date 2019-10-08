#           Škola: Brno University of Technology, Faculty of Information Technology
# Název projektu: IFJ18
#       Předměty: IFJ a IAL
#           Část: Scanner
#            Tým: 027
#       Varianta: I
#         Autoři: Aleš Tetur          (xtetur01)
#                 Šimon Matyáš        (xmatya11)
#                 Jan Tilgner         (xtilgn01)
#                 Jan Bíl             (xbilja00)
#
CC= gcc
CCFLAGS= -std=c99 -Wall -Wextra -pedantic
LDLIBS= -lm
SRC= $(wildcard *.c) 
OBJ= $(SRC:%.c=%.o)


#linking
ifj18: $(OBJ)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDLIBS)

