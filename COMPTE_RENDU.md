# Compte rendu — Sortir du fichier unique

## Exercice 0 — Initialisation

Les fichiers `.o` et les exécutables sont produits par le compilateur. Ils
dépendent du système et peuvent toujours être reconstruits à partir des sources ;
les versionner alourdirait inutilement le dépôt.

## Exercice 1 — Séparer le module

| Question | Réponse |
| --- | --- |
| A | `Maillon` se trouve dans `liste.h` parce que le programme client doit connaître ce type pour déclarer une liste. L'en-tête garantit une définition commune à tous les fichiers. |
| B | `liste.c` inclut son propre en-tête pour que le compilateur vérifie la cohérence entre les prototypes publics et leurs définitions. |

## Exercice 2 — Compilation en deux étapes

Commandes exécutées :

```sh
gcc -Wall -Wextra -Werror -std=c11 -g -c main.c -o main.o
gcc -Wall -Wextra -Werror -std=c11 -g -c liste.c -o liste.o
gcc -Wall -Wextra -Werror -std=c11 -g -o demo main.o liste.o
```

Sortie :

```text
liste       : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
longueur    : 5
contient 30 : oui
liberee
```

| Question | Réponse |
| --- | --- |
| A | Deux fichiers objets sont créés, `main.o` et `liste.o`. Il n'existe pas de `liste.h.o` : un en-tête est inséré par le préprocesseur dans chaque source qui l'inclut, mais n'est pas compilé seul. |
| B | La commande directe qui reçoit les deux `.c` recompile tout. Les étapes séparées permettent de ne reconstruire que les objets affectés par une modification, puis de refaire rapidement le lien. |

