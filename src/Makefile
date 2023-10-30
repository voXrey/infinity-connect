# Nom de l'exécutable
EXEC = pow

# Liste des fichiers sources
SRCS = main.c global.c functions.c menus.c utils.c

# Compilation de l'exécutable
all: $(SRCS)
	gcc -o $(EXEC) $(SRCS) -W

# Nettoyage
clean:
	rm -f $(EXEC) $(SRCS:.c=.o)
