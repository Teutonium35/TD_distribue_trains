all : gestionnaireTrains1 gestionnaireTrains2 gestionnaireRessources

gestionnaireTrains1 : gestionnaireTrains1.c
	gcc $^ -o $@ -lm

gestionnaireTrains2 : gestionnaireTrains2.c
	gcc $^ -o $@ -lm

gestionnaireRessources : gestionnaireRessources.c
	gcc $^ -o $@ -pthread -lm

clean:
	rm -rf gestionnaireTrains1 gestionnaireTrains2 gestionnaireRessources