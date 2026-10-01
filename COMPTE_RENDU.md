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

## Exercice 3 — Reconnaître trois erreurs

Les erreurs ont été provoquées dans des copies temporaires, puis les sources
correctes ont été restaurées.

| Cas | Premier diagnostic utile | Étape |
| --- | --- | --- |
| Objet oublié | `undefined reference to 'liste_inserer'` | Édition de liens |
| En-tête oublié | `main.c:5:5: error: unknown type name 'Maillon'` | Compilation |
| Garde oubliée | `liste.h:3:16: error: redefinition of 'struct Maillon'` | Compilation |

| Question | Réponse |
| --- | --- |
| 1 | Le cas 1 vient de l'éditeur de liens : les appels sont compilés, mais `ld` ne trouve pas leurs définitions puisque `liste.o` manque. Les termes `undefined reference` et `ld returned 1 exit status` le montrent. |
| 2 | `unknown type name 'Maillon'` est le message utile ; les déclarations implicites suivantes ne sont que des conséquences. |
| 3 | Le doublon devient possible lorsqu'un même en-tête est atteint par plusieurs chemins, par exemple si deux modules inclus par `main.c` incluent tous les deux `liste.h`. |

## Exercice 4 — Premier Makefile

Le premier `make` exécute :

```text
gcc -Wall -Wextra -Werror -std=c11 -g -c main.c
gcc -Wall -Wextra -Werror -std=c11 -g -c liste.c
gcc -Wall -Wextra -Werror -std=c11 -g -o demo main.o liste.o
```

| Question | Réponse |
| --- | --- |
| A | Sans modification, toutes les cibles sont plus récentes que leurs dépendances. La seconde exécution répond donc `make: 'demo' is up to date.` |
| B | Quatre espaces ne commencent pas une recette : GNU Make 4.4.1 répond `Makefile:2: *** missing separator. Stop.` |

## Exercice 5 — Dépendance à `liste.h`

| Configuration | Reconstruction après modification de `liste.h` |
| --- | --- |
| Dépendance correcte | `main.o`, `liste.o`, puis `demo` |
| `liste.h` oublié pour `main.o` | `liste.o`, puis `demo` seulement |

| Question | Réponse |
| --- | --- |
| A | Avec la bonne dépendance, les deux fichiers qui incluent l'en-tête sont recompilés. Sans elle, `make` considère à tort que `main.o` est encore valide. |
| B | L'exécutable peut mélanger deux objets compilés avec des définitions différentes de `Maillon`. L'édition de liens peut réussir, mais le comportement du programme devient indéfini. |

## Exercice 6 — Compteur d'allocations

| Mesure | Sans fuite | Avec une seconde liste de 3 éléments oubliée |
| --- | ---: | ---: |
| Après création | 5 | 8 |
| Après libération de la liste principale | 0 | 3 |

| Question | Réponse |
| --- | --- |
| A | Le compteur et les deux enveloppes sont des détails du module. `static` leur donne une liaison interne et empêche le reste du programme de les utiliser directement. |
| B | Incrémenter après un échec de `malloc` compterait un bloc inexistant. Décrémenter pour `NULL` serait également faux puisque `free(NULL)` ne libère aucun bloc. |
| C | Le compteur donne le nombre de blocs manquants, pas leur origine. Il faut observer sa valeur à plusieurs étapes, simplifier le scénario ou ajouter temporairement des traces aux allocations pour trouver l'opération responsable. |

## Exercice 7 — Valgrind

Mesures obtenues avec Valgrind 3.25.1 sur un système Linux 64 bits :

| Mesure | Sans fuite | Avec la fuite |
| --- | ---: | ---: |
| `definitely lost` | 0 | 16 octets dans 1 bloc |
| `indirectly lost` | 0 | 32 octets dans 2 blocs |
| `total heap usage` | 5 allocations, 5 libérations | 8 allocations, 5 libérations |
| Compteur interne final | 0 | 3 |

Sans fuite, les dernières informations sont :

```text
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

La pile de la variante fautive remonte de `malloc` vers `suivi_malloc`, puis
`liste_inserer`, jusqu'à l'appel de `main.c:15` qui construit la seconde liste.

| Question | Réponse |
| --- | --- |
| A | La ligne indiquée est celle de l'allocation devenue inaccessible. L'oubli d'un appel à `liste_liberer` n'a pas de ligne exécutable que l'outil pourrait désigner. |
| B | Le maillon de tête représente 1 bloc directement perdu. Les 2 suivants ne sont joignables qu'à travers lui et sont donc indirectement perdus. |
| C | Ici, les nombres sont 5/5 puis 8/5, car la bibliothèque C utilisée n'alloue pas de tampon supplémentaire pour cette sortie. Sur une autre libc, Valgrind peut compter des allocations internes en plus des maillons. |

## Exercice 8 — Dépassement de tableau

| Observation | Résultat |
| --- | --- |
| Exécution normale | Affiche `42` et retourne 0 |
| Exécution instrumentée | `AddressSanitizer: heap-buffer-overflow` et code 1 |

AddressSanitizer indique une écriture de 4 octets immédiatement après une zone
allouée de 20 octets.

| Question | Réponse |
| --- | --- |
| A | Le programme possède un `malloc` et un `free` : le compteur d'allocations resterait équilibré et ne verrait pas le dépassement. |
| B | Les 20 octets correspondent à 5 entiers de 4 octets. `t[5]` commence exactement après le bloc, donc à 0 octet après sa fin ; `t[6]` commencerait 4 octets plus loin. |
| C | Une sortie attendue et un code nul ne prouvent pas l'absence de comportement indéfini. Un test qui passe peut ne pas exercer ou révéler l'erreur. |
| D | Le compteur reste utile comme invariant léger pour le module. Valgrind ou AddressSanitizer est nécessaire pour localiser les fuites et détecter les accès hors limites ou après libération. |
