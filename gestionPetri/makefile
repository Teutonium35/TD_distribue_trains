CFLAGS = -Wall -Wextra -g -I./include

all : bin/gestionnaireTrains1 bin/gestionnaireTrains2 bin/gestionnaireRessources

bin/gestionnaireTrains1 : src/gestionnaireTrains1.c src/gestionRessourcesTrains.c src/messagerieTrainApi.c
	gcc $(CFLAGS) $^ -o $@ -pthread -lm

bin/gestionnaireTrains2 : src/gestionnaireTrains2.c src/gestionRessourcesTrains.c src/messagerieTrainApi.c
	gcc $(CFLAGS) $^ -o $@ -pthread -lm

bin/gestionnaireRessources : src/gestionnaireRessources.c src/gestionMutex.c src/gestionRessources.c src/gestionCommunication.c
	gcc $(CFLAGS) $^ -o $@ -pthread -lm

clean:
	rm -rf bin/gestionnaireTrains1 bin/gestionnaireTrains2 bin/gestionnaireRessources